// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 *
 */
class UHG_SlotWidget;
class UTextBlock;
class UButton;
class UWidgetSwitcher;
class UImage;
class UWrapBox;
class AHG_EquipItem;
class AHG_Player;

UCLASS()
class METACHEERINGROOM_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	UWidgetSwitcher* WS_Category;

	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	UWrapBox* WB_SlotList_Active;
	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	UWrapBox* WB_SlotList_Costume;
	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	UWrapBox* WB_SlotList_Emotion;
	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	UWrapBox* WB_SlotList_Emoji;
	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	UWrapBox* WB_SlotList_Sound;

	void InitInventoryUI();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHG_SlotWidget> SlotWidgetFactory;

	void SetOwner(APawn* Player);

	UPROPERTY(EditDefaultsOnly)
	UWrapBox* SelectedCategory;

	UPROPERTY(EditDefaultsOnly)
	UHG_SlotWidget* SelectedSlot;

	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	UButton* Btn_ActiveCategory;

	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	UButton* Btn_CostumeCategory;

	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	UButton* Btn_EmotionCategory;

	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	UButton* Btn_EmojiCategory;

	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	UButton* Btn_SoundCategory;

	UPROPERTY(EditDefaultsOnly)
	UMaterial* DefaultImage;

	UFUNCTION()
	void SelectCategory_Active();

	UFUNCTION()
	void SelectCategory_Costume();

	UFUNCTION()
	void SelectCategory_Emotion();

	UFUNCTION()
	void SelectCategory_Emoji();

	UFUNCTION()
	void SelectCategory_Sound();

	UFUNCTION()
	void ThrowAwaySelectedItem();

	UFUNCTION()
	void UseItem();

	bool bSlotSelected = false;

	TArray<UHG_SlotWidget*> EquipList;

	class UHG_GameInstance* GI;

	int32 GetSlotIndexInWB(UWidget* SlotWidget);

	void CheckEquipitem();

	void CheckButtonClick(UButton* p_Button);
};
