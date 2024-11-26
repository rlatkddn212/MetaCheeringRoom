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

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* TB_Point;

	void SetPointText();

	UFUNCTION(BlueprintCallable)
	void UpdateHUD(int32 Value);

	void PlayAppearAnimation(bool Play_Forward);
	void PlayInventoryAnimation();
	void StopInventoryAnimation();

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Appear;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Disappear;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* GetCoin;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* NewItem;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UWidgetSwitcher* WS_Border;
};
