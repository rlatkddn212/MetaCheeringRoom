// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_FeverWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Styling/SlateColor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "JS_CoinActor.h"
#include "../SHK/HG_Player.h"
#include "../SHK/HG_PlayerGoodsComponent.h"
#include "MetaCheeringRoom.h"
void UJS_FeverWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);
	DownPercent(InDeltaTime);
	UpdateGradientColor(InDeltaTime);
	FeverStart();
	TextGradientColor(InDeltaTime);
	if(bFeverStart || bFeverTime)
	{ 
		CurrentTime += InDeltaTime;
		// 남은 시간 감소
		if (bFeverStart)
		{
			RemainingTime -= InDeltaTime;
			if (RemainingTime <= 0.f)
			{
				RemainingTime = 0.f;
			}

			// 초와 밀리초 계산
			int32 Seconds = FMath::FloorToInt(RemainingTime);
			int32 Milliseconds = FMath::FloorToInt((RemainingTime - Seconds) * 100);

			// 시간 텍스트 업데이트
			FString FormattedTime = FString::Printf(TEXT("%02d:%02d"), Seconds, Milliseconds);

			if (TEXT_Time)
			{
				TEXT_Time->SetText(FText::FromString(FormattedTime));
			}
		}

		if (CurrentTime >= FeverEndTime)
		{
			UGameplayStatics::PlaySound2D(GetWorld(),DrumHitSound);
			bFeverTime = false;
			bFeverStart = false;
			SetVisibility(ESlateVisibility::Hidden);
			PB_FeverBar->SetVisibility(ESlateVisibility::Visible);
			CurrentTime = 0.f;
			AHG_Player* Player = Cast<AHG_Player>(GetWorld()->GetFirstPlayerController()->GetCharacter());

			if (Player)
			{
				Player->EndFeverTime();
			}
		}
	}
}

void UJS_FeverWidget::SetPercent(float value)
{
	PB_FeverBar->SetPercent(value);
}

float UJS_FeverWidget::GetPercent()
{
	return PB_FeverBar->GetPercent();
}

void UJS_FeverWidget::DownPercent(float InDeltaTime)
{
	if (PB_FeverBar->GetPercent() <= 0)
	{
		return;
	}
	PB_FeverBar->SetPercent(PB_FeverBar->GetPercent()-(0.01*InDeltaTime));
}

void UJS_FeverWidget::UpdateGradientColor(float InDeltaTime)
{
	if (bFeverTime)
	{
		// 시간을 누적
		TimeElapsed += InDeltaTime;

		// 무지갯빛 색상을 계산
		float Hue = FMath::Fmod(TimeElapsed / ColorChangeSpeed, 1.0f); // 0~1 사이 값 반복
		CurrentColor = FLinearColor::MakeFromHSV8(Hue * 360, 255, 255); // HSV에서 무지갯빛 생성
		if (IMG_Fever)
		{
			IMG_Fever->SetColorAndOpacity(CurrentColor);
		}
		RandomCoinSpawn();
	}
}

void UJS_FeverWidget::FeverStart()
{
	if (bFeverStart == true && bFeverTime == false && PB_FeverBar->GetPercent() >= 1.f)
	{
		PB_FeverBar->SetVisibility(ESlateVisibility::Hidden);
		IMG_Fever->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		bFeverTime = true;
		bFeverStart = false;
		// 남은 시간 초기화
		// 시간 텍스트 초기화
		if (TEXT_Time)
		{
			TEXT_Time->SetText(FText::FromString(TEXT("00:05")));
		}
		CurrentTime = 2.f;
		UGameplayStatics::PlaySound2D(GetWorld(), DrumHitSound);
		UGameplayStatics::PlaySound2D(GetWorld(), FeverSound);
		AHG_Player* Player = Cast<AHG_Player>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		if (Player)
		{
			UHG_PlayerGoodsComponent* Goods = Player->GoodsComp;
			if (Goods)
			{
				Goods->GoldLerp(Goods->GetGold()+1000, 0.1f);
			}
		}
	}
}

void UJS_FeverWidget::TextGradientColor(float InDeltaTime)
{
	// 시간을 누적
	TimeElapsed2 += InDeltaTime;

	// 무지갯빛 색상을 계산
	float Hue = FMath::Fmod(TimeElapsed2 / ColorChangeSpeed2, 1.0f); // 0~1 사이 값 반복
	CurrenTextColor = FLinearColor::MakeFromHSV8(Hue * 360, 255, 255); // HSV에서 무지갯빛 생성
	if (IMG_FeverTime)
	{
		IMG_FeverTime->SetColorAndOpacity(CurrenTextColor);
		if (bFeverTime)
		{
			TEXT_Time->SetColorAndOpacity(CurrenTextColor);
		}
	}
}

void UJS_FeverWidget::RandomCoinSpawn()
{
	ACharacter* Player = GetWorld()->GetFirstPlayerController()->GetCharacter();

	if (Player)
	{
		FVector Loc = Player->GetActorLocation();
		Loc.Z -= 100.f;
		float Min = 300.f;
		float Max = 500.f;
		// 무작위 반지름 값
		float RandomRadius = FMath::RandRange(Min, Max);

		// 구의 무작위 방향
		FVector RandomDirection = FMath::VRand();

		// 위쪽 반구로 제한
		RandomDirection.Z = FMath::Abs(RandomDirection.Z); // Z 값을 양수로 제한

		// 랜덤 방향에 반지름을 곱해 위치 계산
		FVector RandomPoint = Loc + RandomDirection * RandomRadius;
		FRotator SpawnRotation = FRotator::ZeroRotator;
		GetWorld()->SpawnActor<AJS_CoinActor>(CoinFactory, RandomPoint, SpawnRotation);
	}

}
