// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SW_CameraSpeedWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_CameraSpeedWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetCameraSpeed(float Speed);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CameraSpeedText;

	// Show Animation

	UPROPERTY( meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* ShowCameraSpeed;
};
