// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "JS_TotoActor.h"
#include "JS_Screen.h"
#include "JS_NetComponent.h"
#include "JS_ExitWidget.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/OnlineSubsystem.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineIdentityInterface.h"
#include "Net/UnrealNetwork.h"
#include "../SHK/HG_Player.h"
#include "JS_AtkActor.h"
#include "JS_StarActor.h"
#include "MetaCheeringRoom.h"
#include "JS_LoadActor.h"
#include "JS_ChattingWidget.h"
#include "MetaCheeringRoom.h"
#include "../SHK/HG_GameInstance.h"
#include "JS_SessionGameInstanceSubSystem.h"
#include "MediaPlayer.h"

AJS_PlayerController::AJS_PlayerController()
{
	bReplicates = true;
}

void AJS_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		TotoActor = Cast<AJS_TotoActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_TotoActor::StaticClass()));
		ScreenActor = Cast<AJS_Screen>(UGameplayStatics::GetActorOfClass(GetWorld(),AJS_Screen::StaticClass()));
	}
	else
	{
		MyScreenActor = Cast<AJS_Screen>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_Screen::StaticClass()));
	}

	if (ExitWidgetFactory)
	{
		ExitWidget = CreateWidget<UJS_ExitWidget>(GetWorld(), ExitWidgetFactory);
		if (ExitWidget)
		{
			ExitWidget->AddToViewport(15);
			ExitWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	if (GetWorld()->GetMapName() == TEXT("HG_LobbyLevel_New"))
	{
		UHG_GameInstance* gi = Cast<UHG_GameInstance>(GetWorld()->GetGameInstance());
		if (gi)
		{
			UJS_SessionGameInstanceSubSystem* si = gi->GetSubsystem<UJS_SessionGameInstanceSubSystem>();
			if (si)
			{
				si->MySessionDestroy();
			}
		}
	}

}

void AJS_PlayerController::Tick(float Deltatime)
{
	Super::Tick(Deltatime);
	OnPlayerModify(Deltatime);
}

void AJS_PlayerController::ServerHandleBettingToTo_Implementation(int32 point, int32 select, const FString& UserId)
{
	if (TotoActor)
	{
		TotoActor->ServerBettingToto(point, select, UserId);
	}
	else
	{
		TotoActor = Cast<AJS_TotoActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_TotoActor::StaticClass()));
		if (TotoActor)
		{
			TotoActor->ServerBettingToto(point, select, UserId);
		}
	}
}


void AJS_PlayerController::ServerHandleVideoPlay_Implementation()
{
	if (ScreenActor)
	{
		FString URL = ScreenActor->NetComp->VideoURL;
		FString StreamID = ScreenActor->NetComp->StreamID;
		int32 SegmentNumber = ScreenActor->NetComp->SegmentNumber - 1;
		ClientHandleVideoPlay(URL, StreamID, SegmentNumber);
	}
}

void AJS_PlayerController::ServerVODDataReq_Implementation()
{
	if (ScreenActor->MediaPlayer && ScreenActor->MediaPlayer->IsPlaying())
	{
		FTimespan Time = ScreenActor->MediaPlayer->GetTime();
		ClientVodJump(Time);
	}
}

void AJS_PlayerController::ClientVodJump_Implementation(FTimespan Time)
{
	if (MyScreenActor)
	{
		MyScreenActor->MediaPlayer->Seek(Time);
	}
}

void AJS_PlayerController::ClientHandleVideoPlay_Implementation(const FString& URL, const FString& streamID, int32 segmentNumber)
{
	if (MyScreenActor)
	{
		MyScreenActor->NetComp->RepVideoURLWrite(URL,streamID,segmentNumber);
		MyScreenActor->PlayMedia(streamID);
	}
}

void AJS_PlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AJS_PlayerController, MyUserID);
}

void AJS_PlayerController::PlayerModify()
{
	// 플레이어를 가져와서
	if (!Player)
	{
		Player = Cast<AHG_Player>(GetCharacter());
	}

	if (Player)
	{
		if (Player->IsLocallyControlled())
		{
			UGameplayStatics::PlaySound2D(GetWorld(), SpringSound);
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

void AJS_PlayerController::OnPlayerModify(float DeltaTime)
{
	if (!Player)
	{
		return;
	}
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

void AJS_PlayerController::SpawnStarActor()
{
	if (Player && StarActorFactory)
	{
		// 스켈레탈 메시의 헤드 소켓 위치 가져오기
		FVector SpLocation = Player->GetMesh()->GetSocketLocation(FName("head"));
		SpLocation.Z += 20.f;
		FRotator SpawnRotation = Player->GetActorRotation();

		// 스타 액터 스폰
		FActorSpawnParameters SpawnParams;
		StarActor = GetWorld()->SpawnActor<AJS_StarActor>(StarActorFactory, SpLocation, SpawnRotation, SpawnParams);
	}
}

void AJS_PlayerController::SetMyUserID_Implementation(const FString& str)
{
	MyUserID = str;
}

void AJS_PlayerController::ServerAddChat_Implementation(const FString& id, const FText& text, bool bAuto)
{
	if (!LoadActor)
	{
		LoadActor = Cast<AJS_LoadActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_LoadActor::StaticClass()));
	}
	if (LoadActor)
	{
		if (LoadActor->GetOwner() == this)
		{
			LoadActor->MulticastAutoChatOpen();
		}
		LoadActor->MulticastAddChat(id,text,bAuto);
	}
}

void AJS_PlayerController::ClientAddChat_Implementation(const FString& id, const FText& text, bool bAuto)
{
	if (!LoadActor)
	{
		LoadActor = Cast<AJS_LoadActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_LoadActor::StaticClass()));
	}
	if (LoadActor && LoadActor->ChatWidget)
	{
		LoadActor->ChatWidget->AddChat(id, text, bAuto);
	}
}

void AJS_PlayerController::PlayUISound()
{
	if (UIShowAndHideSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(),UIShowAndHideSound);
	}
}
