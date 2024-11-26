// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HG_FullScreenWidget.generated.h"

/**
 *
 */
UCLASS()
class METACHEERINGROOM_API UHG_FullScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Appear;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Disappear;
};
