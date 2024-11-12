// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_TotoActor.h"
#include "ToToMakeWidget.h"
#include "JS_ToToWidget.h"
#include "Online/CoreOnline.h"
#include "OnlineSubSystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystemUtils.h"
#include "../MetaCheeringRoom.h"
#include "JS_GameState.h"
#include "JS_PlayerController.h"
#include "../SHK/HG_Player.h"
#include "../SHK/HG_PlayerGoodsComponent.h"
#include "JS_AtkActor.h"
#include "JS_AtkAnimInstance.h"
#include "Components/TimelineComponent.h"
#include "JS_StarActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AJS_TotoActor::AJS_TotoActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AJS_TotoActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		SetOwner(PC->GetPawn());
		if (TotoMakeWidgetFactory)
		{
			TotoMakeWidget = CreateWidget<UToToMakeWidget>(GetWorld(), TotoMakeWidgetFactory);
			if (TotoMakeWidget)
			{
				TotoMakeWidget->AddToViewport(30);
				TotoMakeWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		if (ToToWidgetFactory)
		{
			ToToWidget = CreateWidget<UJS_ToToWidget>(GetWorld(), ToToWidgetFactory);
			if (ToToWidget)
			{
				ToToWidget->AddToViewport(29);
				//ToToWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
	else
	{
		if (ToToWidgetFactory)
		{
			ToToWidget = CreateWidget<UJS_ToToWidget>(GetWorld(),ToToWidgetFactory);
			if (ToToWidget)
			{
				ToToWidget->AddToViewport(29);
				//ToToWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
	// Online Subsystem 인스턴스 가져오기
	IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get(TEXT("Steam"));
	if (SubSystem)
	{
		// Identity 인터페이스 가져오기
		IOnlineIdentityPtr IdentityInterface = SubSystem->GetIdentityInterface();
		if (IdentityInterface.IsValid())
		{
			// 로컬 플레이어의 유니크 넷 ID 가져오기
			FUniqueNetIdPtr UserId = IdentityInterface->GetUniquePlayerId(0);
			if (UserId.IsValid())
			{
				// Steam ID를 문자열로 변환
				MyUserID = UserId->ToString();
			}
		}
	}
}

void AJS_TotoActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	OnPlayerModify(DeltaTime);
}

void AJS_TotoActor::MakeToto(FString totoName, FString select1, FString select2, int32 second)
{
	TotoName = totoName;
	Select1 = select1;
	Select2 = select2;
	TotoLimitTIme = second;
	Betting1.Empty();
	Betting2.Empty();
	ServerSetTimerLimit();
	MulticastInitToto();
	MulticastAlarmToto(TEXT("승부예측이 시작되었습니다!"));
	MulticastSetToToUI(totoName,select1,select2);
}

void AJS_TotoActor::MulticastSetToToUI_Implementation(const FString& totoName, const FString& select1, const FString& select2, int32 second, int32 totalSelect1 /*= 0*/, int32 totalSelect2 /*= 0*/, int32 totalBettor1 /*= 0*/, int32 totalBettor2 /*= 0*/, float totalOdds1 /*= 1.f*/, float totalOdds2 /*= 1.f*/)
{
	// ToTo UI에 값들을 세팅해주기
	if (ToToWidget)
	{
		ToToWidget->SetTotoInfo(totoName,select1,select2,totalSelect1,totalSelect2,totalBettor1,TotalBettor2,totalOdds1,totalOdds2);
	}
}

void AJS_TotoActor::MulticastAlarmToto_Implementation(const FString& AlarmText)
{
	if (ToToWidget)
	{
		ToToWidget->AlarmToto(AlarmText);
	}
}

void AJS_TotoActor::BettingToto(int32 point, int32 select)
{
	MyPoint += point;
	MySelect = select;
	AJS_PlayerController* PC = Cast<AJS_PlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC && PC->HasAuthority())
	{
		ServerBettingToto(point, select, MyUserID);
	}
	else
	{
		PC->ServerHandleBettingToTo(point,select,MyUserID);
	}

}

void AJS_TotoActor::ServerBettingToto_Implementation(int32 point, int32 select, const FString& BettorID)
{

	// 배팅한 사람 Map에 포인트 추가하기
	if (select == 1)
	{
		TotalSelect1+=point;
		if (Betting1.Contains(BettorID))
		{
			Betting1[BettorID] += point;
		}
		else
		{
			Betting1.Add(BettorID,point);
		}
	}
	else
	{
		TotalSelect2+=point;
		if (Betting2.Contains(BettorID))
		{
			Betting2[BettorID] += point;
		}
		else
		{
			Betting2.Add(BettorID, point);
		}
	}
	int32 Div1 = TotalSelect1;
	int32 Div2 = TotalSelect2;
	if (TotalSelect1 == 0)
	{
		Div1 = 1;
	}
	if (TotalSelect2 == 0)
	{
		Div2 = 1;
	}
	//MulticastSetToToUI 배당률을 계산해서 넣어주고
	TotalOdds1 = ((float)TotalSelect1 + TotalSelect2) / Div1;
	TotalOdds2 = ((float)TotalSelect1 + TotalSelect2) / Div2;

	TotalBettor1 = Betting1.Num();
	TotalBettor2 = Betting2.Num();

	MulticastSetToToUI(TotoName,Select1,Select2,-1,TotalSelect1,TotalSelect2,TotalBettor1,TotalBettor2,TotalOdds1,TotalOdds2);
}

void AJS_TotoActor::AdjustPoint(int32 ResultNum)
{
	// 멀티캐스트로 예측 결과 안내
	// 각 플레이어의 게임 인스턴스에 포인트를 지급하기 ( 멀티캐스트, 인자값으로 넘겨주기? )
	TArray<FString> Keys;
	TArray<int32> Values;
	TArray<FString> LoseKeys;
	TArray<int32> LoseValues;
	if (ResultNum == 1)
	{
		for (auto& Elem : Betting1)
		{
			Keys.Add(Elem.Key);
			Values.Add(Elem.Value);
		}
		for (auto& Elem : Betting2)
		{
			LoseKeys.Add(Elem.Key);
			LoseValues.Add(Elem.Value);
		}
	}
	else
	{
		for (auto& Elem : Betting2)
		{
			Keys.Add(Elem.Key);
			Values.Add(Elem.Value);
		}
		for (auto& Elem : Betting1)
		{
			LoseKeys.Add(Elem.Key);
			LoseValues.Add(Elem.Value);
		}
	}
	MulticastAdjustPoint(Keys, Values, TotalOdds1);
	MulticastAdjustLose(LoseKeys, LoseValues);
}

void AJS_TotoActor::MulticastAdjustPoint_Implementation(const TArray<FString>& Keys, const TArray<int32>& Values, float Odd)
{
	int32 BettingPoint = 0;
	bool bWin  =false;
	for (int32 i=0;i<Keys.Num();i++)
	{
		if (Keys[i] == MyUserID)
		{
			BettingPoint = Values[i];
			bWin = true;
			break;
		}
	}
	Player = Cast<AHG_Player>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	Player->GoodsComp->AddGold(BettingPoint * Odd);

	if (bWin == true)
	{
		AdjustWin();
	}

}

void AJS_TotoActor::MulticastAdjustLose_Implementation(const TArray<FString>& Keys, const TArray<int32>& Values)
{
	bool bLose = false;
	for (int32 i = 0; i < Keys.Num(); i++)
	{
		if (Keys[i] == MyUserID)
		{
			bLose = true;
			break;
		}
	}
	// 만약 졌다면
	if (bLose)
	{
		// 실패 애니메이션 시작
		LoseAnimationPlay();
	}
}

void AJS_TotoActor::AdjustWin()
{
	if (!Player)
	{
		return;
	}
	
	if (Player && Player->IsLocallyControlled())
	{
		// 축하 사운드 재생
		UGameplayStatics::PlaySound2D(GetWorld(),TadaSound);
	}

	FVector SpawnLocation = Player->GetActorLocation(); // Offset distance
	SpawnLocation.Z += 50;
	FRotator SpawnRotation = FRotator::ZeroRotator;

	if (FanfareFactory)
	{
		AActor* Actor = GetWorld()->SpawnActor<AActor>(FanfareFactory, SpawnLocation, SpawnRotation);
	}
}

void AJS_TotoActor::ServerSetTimerLimit()
{
	if (TotoLimitTIme < 0)
	{
		FString timeLimitText = FString::Printf(TEXT("제출이 마감되었습니다."));
		MulticastSetTimeUI(timeLimitText, TotoLimitTIme);
		MulticastAlarmToto(TEXT("승부예측이 마감되었습니다!"));

	}
	else
	{
		int32 min = TotoLimitTIme / 60;
		int32 sec = TotoLimitTIme % 60;
		FString timeLimitText = FString::Printf(TEXT("%d:%2d 후에 제출이 마감됩니다."), min, sec);
		TotoLimitTIme--;
		MulticastSetTimeUI(timeLimitText, TotoLimitTIme);
		GetWorld()->GetTimerManager().SetTimer(ToToLimitTimerHandle, this, &AJS_TotoActor::ServerSetTimerLimit, 1.f, false);
	}
}

void AJS_TotoActor::MulticastSetTimeUI_Implementation(const FString& TimeText, const int32& Time)
{
	if (Time < 0)
	{
		if (ToToWidget)
		{
			// 더이상 예측 못하게 UI 바뀌기
			ToToWidget->SetTimerText(TimeText);
			ToToWidget->SetBettingStopUI();
		}
		if (TotoMakeWidget)
		{
			TotoMakeWidget->SetWidgetSwitcher(1);
			TotoMakeWidget->bOpen = true;
		}
	}
	else
	{
		if (ToToWidget)
		{
			ToToWidget->SetTimerText(TimeText);
		}
	}

}

void AJS_TotoActor::MulticastInitToto_Implementation()
{

	// 전체 포인트
	TotalSelect1 = 0;
	TotalSelect2 = 0;
	// 전체 참여자
	TotalBettor1 = 0;
	TotalBettor2 = 0;
	// 전체 배당
	TotalOdds1 = 0.f;
	TotalOdds2 = 0.f;
	// 내 선택
	MySelect = -1;

	if (ToToWidget)
	{
		ToToWidget->InitWidget();
	}
}

void AJS_TotoActor::LoseAnimationPlay()
{
	// 플레이어를 가져와서
	Player = Cast<AHG_Player>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	// 플레이어 앞에 액터를 소환하기, 플레이어에게 Attach하기
	if (Player)
	{
		if (Player->IsLocallyControlled())
		{
			UGameplayStatics::PlaySound2D(GetWorld(), PoofSound);
		}
		FVector SpawnLocation = Player->GetActorLocation(); // Offset distance
		SpawnLocation.Z += 50;
		FRotator SpawnRotation = FRotator::ZeroRotator;

		if (AtkActorFactory)
		{
			AActor* Actor = GetWorld()->SpawnActor<AActor>(AtkActorFactory, SpawnLocation, SpawnRotation);

			AtkActor = Cast<AJS_AtkActor>(Actor);
			if (AtkActor)
			{
				UJS_AtkAnimInstance* AnimIns = Cast<UJS_AtkAnimInstance>(AtkActor->SkelMesh->GetAnimInstance());
				if (AnimIns)
				{
					AnimIns->SetTotoActor(this);
				}
			}
		}
	}
}

void AJS_TotoActor::PlayerModify()
{
	// 플레이어를 가져와서
	if(!Player)
	{
		Player = Cast<AHG_Player>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	}
	
	if (Player)
	{
		if (Player->IsLocallyControlled())
		{
			UGameplayStatics::PlaySound2D(GetWorld(),SpringSound);
		}
		OriginalScale = Player->GetMesh()->GetRelativeScale3D();
		// 움직임 제한 시작
		Player->bCanMove = false;
		StunTime = 0.0f;
		// 스타 액터 스폰
		SpawnStarActor();
	}
	if (!bAnimating)
	{
		bAnimating = true;
		CurrentTime = 0.0f;
		CurrentPhase = EModifyPhase::Squashing;
	}
}

void AJS_TotoActor::OnPlayerModify(float DeltaTime)
{

	// 스턴 타이머 업데이트
	if (Player && !Player->bCanMove)
	{
		StunTime += DeltaTime;
		if (StunTime >= StunDuration)
		{
			Player->bCanMove = true;  // 움직임 다시 활성화
			StunTime = -999999999;
		}
	}
	if (StarActor)
	{
		FVector PlayerHeadLocation = Player->GetMesh()->GetSocketLocation(FName("head"));
		PlayerHeadLocation.Z += 20;
		StarActor->SetActorLocation(PlayerHeadLocation);
	}
	if (AtkActor)
	{
		FVector PlayerLocation = Player->GetActorLocation();
		PlayerLocation.Z += 50;
		AtkActor->SetActorLocation(PlayerLocation);
	}
	if (!bAnimating || !Player)
	{
		return;
	}
	CurrentTime += DeltaTime;
	FVector NewScale = OriginalScale;

	switch (CurrentPhase)
	{
	case EModifyPhase::Squashing:
	{
		float Alpha = FMath::Clamp(CurrentTime / (AnimationDuration * 1.2f), 0.0f, 1.0f);

		if (Alpha < 0.4f)  // 첫 단계: 매우 심하게 찌그러짐
		{
			float InitialSquashAlpha = Alpha * 2.5f;  // 0~0.4를 0~1로 변환
			float ExtremeSquash = Squash * 0.5f;  // 더 심하게 찌그러짐 (Squash보다 더 작은 값)

			NewScale.Z *= FMath::Lerp(1.0f, ExtremeSquash, InitialSquashAlpha);
			NewScale.X *= FMath::Lerp(1.0f, 0.3f / ExtremeSquash, InitialSquashAlpha);
			NewScale.Y *= FMath::Lerp(1.0f, 0.3f / ExtremeSquash, InitialSquashAlpha);
		}
		else  // 두 번째 단계: 적당한 찌그러짐으로 돌아옴
		{
			float RecoverAlpha = (Alpha - 0.4f) * 1.67f;  // 0.4~1을 0~1로 변환
			float ExtremeSquash = Squash * 0.5f;

			NewScale.Z *= FMath::Lerp(ExtremeSquash, Squash, RecoverAlpha);
			NewScale.X *= FMath::Lerp(1.2f / ExtremeSquash, 0.25f / Squash, RecoverAlpha);
			NewScale.Y *= FMath::Lerp(1.2f / ExtremeSquash, 0.25f / Squash, RecoverAlpha);
		}

		if (Alpha >= 1.0f)
		{
			CurrentPhase = EModifyPhase::Holding;
			CurrentTime = 0.0f;
		}
		break;
	}
	case EModifyPhase::Holding:
	{
		// 찌그러진 상태 유지
		NewScale.Z *= Squash;
		NewScale.X *= 0.25f / Squash;
		NewScale.Y *= 0.25f / Squash;

		if (CurrentTime >= HoldTime)
		{
			CurrentPhase = EModifyPhase::Recovering;
			CurrentTime = 0.0f;
		}
		break;
	}
	case EModifyPhase::Recovering:
	{
		float Alpha = FMath::Clamp(CurrentTime / (AnimationDuration * 1.5f), 0.0f, 1.0f);

		if (Alpha < 0.5f)  // 첫 번째 단계: 길게 늘어나기 (띠용~)
		{
			float StretchAlpha = Alpha * 2.0f;  // 0~0.5를 0~1로 변환
			float StretchIntensity = 1.8f;  // 늘어나는 정도

			NewScale.Z *= FMath::Lerp(Squash, StretchIntensity, StretchAlpha);
			NewScale.X *= FMath::Lerp(1.0f / Squash, 0.7f / StretchIntensity, StretchAlpha);
			NewScale.Y *= FMath::Lerp(1.0f / Squash, 0.7f / StretchIntensity, StretchAlpha);
		}
		else  // 두 번째 단계: 원래 크기로 돌아오기
		{
			float RecoverAlpha = (Alpha - 0.5f) * 2.0f;  // 0.5~1을 0~1로 변환
			float StretchIntensity = 1.8f;

			NewScale.Z *= FMath::Lerp(StretchIntensity, 1.0f, RecoverAlpha);
			NewScale.X *= FMath::Lerp(0.7f / StretchIntensity, 1.0f, RecoverAlpha);
			NewScale.Y *= FMath::Lerp(0.7f / StretchIntensity, 1.0f, RecoverAlpha);
		}

		if (Alpha >= 1.0f)
		{
			bAnimating = false;
			CurrentPhase = EModifyPhase::None;
			NewScale = OriginalScale;  // 완전히 정확한 원래 크기로 복구
		}
		break;
		}
	}
	Player->GetMesh()->SetRelativeScale3D(NewScale);
}

void AJS_TotoActor::SpawnStarActor()
{
	if (Player && StarActorFactory)
	{
		// 스켈레탈 메시의 헤드 소켓 위치 가져오기
		FVector SpawnLocation = Player->GetMesh()->GetSocketLocation(FName("head"));
		SpawnLocation.Z += 20.f;
		FRotator SpawnRotation = Player->GetActorRotation();

		// 스타 액터 스폰
		FActorSpawnParameters SpawnParams;
		StarActor = GetWorld()->SpawnActor<AJS_StarActor>(StarActorFactory, SpawnLocation, SpawnRotation, SpawnParams);
	}
}