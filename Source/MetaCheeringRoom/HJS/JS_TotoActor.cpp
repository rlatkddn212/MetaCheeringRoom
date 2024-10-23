// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_TotoActor.h"
#include "ToToMakeWidget.h"
#include "JS_ToToWidget.h"

// Sets default values
AJS_TotoActor::AJS_TotoActor()
{

}

// Called when the game starts or when spawned
void AJS_TotoActor::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	SetOwner(PC);

	if (HasAuthority())
	{
		if (TotoMakeWidgetFactory)
		{
			TotoMakeWidget = CreateWidget<UToToMakeWidget>(GetWorld(), TotoMakeWidgetFactory);
			if (TotoMakeWidget)
			{
				TotoMakeWidget->AddToViewport();
				TotoMakeWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		if (ToToWidgetFactory)
		{
			ToToWidget = CreateWidget<UJS_ToToWidget>(GetWorld(), ToToWidgetFactory);
			if (ToToWidget)
			{
				ToToWidget->AddToViewport();
				ToToWidget->SetVisibility(ESlateVisibility::Hidden);
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
				ToToWidget->AddToViewport();
				ToToWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void AJS_TotoActor::MakeToto(FString totoName, FString select1, FString select2)
{
	TotoName = totoName;
	Select1 = select1;
	Select2 = select2;
	MulticastSetToToUI(totoName,select1,select2);
	AlarmToto(TEXT("승부예측이 시작되었습니다!"));
}

void AJS_TotoActor::MulticastSetToToUI_Implementation(const FString& totoName, const FString& select1, const FString& select2, int32 totalSelect1 /*= 0*/, int32 totalSelect2 /*= 0*/, int32 totalBettor1 /*= 0*/, int32 totalBettor2 /*= 0*/, float totalOdds1 /*= 1.f*/, float totalOdds2 /*= 1.f*/)
{
	// ToTo UI에 값들을 세팅해주기
	
}

void AJS_TotoActor::AlarmToto(const FString& AlarmText)
{
	// Text로 Text를 설정해주고
	
	// Tost Visible 처리 하고

	// UI Animation 재생하기
}

void AJS_TotoActor::BettingToto(int32 point, int32 select)
{
	MyPoint += point;
	MySelect = select;
	ServerBettingToto(point, select);
}

void AJS_TotoActor::ServerBettingToto_Implementation(int32 point, int32 select)
{
	if (select == 1)
	{
		TotalSelect1+=point;
	}
	else
	{
		TotalSelect2+=point;
	}
	//MulticastSetToToUI 배당률을 계산해서 넣어주고

	TotalOdds1 = (TotalSelect1 + TotalSelect2) / TotalSelect1;

	TotalOdds2 = (TotalSelect1 + TotalSelect2) / TotalSelect2;

}
