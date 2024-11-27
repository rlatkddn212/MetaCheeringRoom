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
	class UWidgetAnimation* InputG;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* InputL;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* InputV;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* InputT;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* InputEnter;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* TBSlide;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UWidgetSwitcher* WS_Border;


	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Btn_Custom_L;

	UFUNCTION()
	void CustomButton();

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_CheerStick_L;
	UFUNCTION()
	void RCSButton();

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_Inventory_L;

	UFUNCTION()
	void InvenButton();

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_Teleport1;

	UFUNCTION()
	void Tele1Button();

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_Teleport2;

	UFUNCTION()
	void Tele2Button();

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_Teleport3;

	UFUNCTION()
	void Tele3Button();

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_Custom_R;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_CheerStick_R;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_Inventory_R;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_Video;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_Predict;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_FullScreen;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_MicToggle;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_Chat;
};
