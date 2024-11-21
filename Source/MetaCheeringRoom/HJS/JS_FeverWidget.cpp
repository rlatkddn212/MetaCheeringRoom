// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_FeverWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Styling/SlateColor.h"

void UJS_FeverWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);
	DownPercent(InDeltaTime);
	UpdateGradientColor(InDeltaTime);
	FeverStart();
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
	}
}

void UJS_FeverWidget::FeverStart()
{
	if (bFeverStart == true && bFeverTime == false && PB_FeverBar->GetPercent() >= 1.f)
	{
		PB_FeverBar->SetVisibility(ESlateVisibility::Hidden);
		bFeverTime = true;
		bFeverStart = false;
	}
}
