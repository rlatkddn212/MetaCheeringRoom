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

// Sets default values
AJS_TotoActor::AJS_TotoActor()
{

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

void AJS_TotoActor::MakeToto(FString totoName, FString select1, FString select2, int32 second)
{
	TotoName = totoName;
	Select1 = select1;
	Select2 = select2;
	TotoLimitTIme = second;
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
	if (ResultNum == 1)
	{
		for (auto& Elem : Betting1)
		{
			Keys.Add(Elem.Key);
			Values.Add(Elem.Value);
		}
		MulticastAdjustPoint(Keys, Values, TotalOdds1);
	}
	else
	{
		for (auto& Elem : Betting2)
		{
			Keys.Add(Elem.Key);
			Values.Add(Elem.Value);
		}
		MulticastAdjustPoint(Keys, Values, TotalOdds2);
	}

	
}

void AJS_TotoActor::MulticastAdjustPoint_Implementation(const TArray<FString>& Keys, const TArray<int32>& Values, float Odd)
{
	int32 BettingPoint = 0;
	for (int32 i=0;i<Keys.Num();i++)
	{
		if (Keys[i] == MyUserID)
		{
			BettingPoint = Values[i];
			break;
		}
	}
	AHG_Player* Player = Cast<AHG_Player>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	Player->GoodsComp->AddGold(BettingPoint * Odd);
}

void AJS_TotoActor::ServerSetTimerLimit()
{
	if (TotoLimitTIme <= 0)
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
	if (Time <= 0)
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
