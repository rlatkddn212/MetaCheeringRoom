// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_CameraSpeedWidget.h"
#include "Components/TextBlock.h"

void USW_CameraSpeedWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
}

void USW_CameraSpeedWidget::SetCameraSpeed(float Speed)
{
	// 10.0 x 소수점 2자리까지 표시
	FString SpeedText = FString::Printf(TEXT("%.2f x"), Speed);
	CameraSpeedText->SetText(FText::FromString(SpeedText));

	PlayAnimation(ShowCameraSpeed);
}
