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

	void InitInventoryUI();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHG_SlotWidget> SlotWidgetFactory;

	void SetOwner(APawn* Player);
	
	UPROPERTY(EditDefaultsOnly)
	UWrapBox* SelectedCategory;

	UPROPERTY(EditDefaultsOnly)
	UHG_SlotWidget* SelectedSlot;

	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	UImage* Img_SelectedItem;

	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	UTextBlock* TB_ItemName;

	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	UTextBlock* TB_Price;

	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	UTextBlock* TB_Quantity;

	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	UButton* Btn_Use;

	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	UButton* Btn_ThrowAway;

	
	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	UButton* Btn_ActiveCategory;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	UButton* Btn_CostumeCategory;
	
	UPROPERTY(EditDefaultsOnly)
	UMaterial* DefaultImage;

	UFUNCTION()
	void SelectCategory_Active();

	UFUNCTION()
	void SelectCategory_Costume();


	void DIsplaySelectedItemInfo();

	UFUNCTION()
	void ThrowAwaySelectedItem();

	UFUNCTION()
	void UseItem();
};
