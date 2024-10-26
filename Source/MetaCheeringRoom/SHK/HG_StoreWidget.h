// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HG_StoreWidget.generated.h"

/**
 *
 */
UCLASS()
class METACHEERINGROOM_API UHG_StoreWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly)
	class UTextBlock* TB_Point;

	void SetPointText(int32 Value);
};
