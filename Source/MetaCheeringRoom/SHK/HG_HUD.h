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

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* TB_Point;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* TB_Animation;

	void SetPointText();

	void SetAnimPointText(int32 Value);

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

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Input1;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Input2;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Input3;	
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* InputInven;	
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* InputCustom;	
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* InputCS;	
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* TBSlide;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UWidgetSwitcher* WS_Border;
};
