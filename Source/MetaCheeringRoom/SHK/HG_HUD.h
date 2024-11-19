// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HG_HUD.generated.h"

/**
 *
 */
UCLASS()
class METACHEERINGROOM_API UHG_HUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* TB_RemoteCS;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* TB_Point;
	
	void SetPointText();

	void UpdateHUD(FString Value);
};
