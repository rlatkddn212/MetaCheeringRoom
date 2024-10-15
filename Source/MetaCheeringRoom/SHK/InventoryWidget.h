// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 *
 */
UCLASS()
class METACHEERINGROOM_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativeConstruct() override;


	UPROPERTY()
	class UWrapBox* WB_SlotList;

	void InitInventoryUI();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UHG_SlotWidget> SlotWidgetFactory;

	void SetOwner(APawn* Player);

	UPROPERTY(EditDefaultsOnly)
	UHG_SlotWidget* SelectedSlot;

	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	class UImage* Img_SelectedItem;

	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	class UTextBlock* TB_ItemName;

	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	class UTextBlock* TB_Price;

	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	class UTextBlock* TB_Quantity;

	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	class UButton* Btn_Use;

	UPROPERTY(EditDefaultsOnly, meta = (BindWIdget))
	class UButton* Btn_ThrowAway;

	void DIsplaySelectedItemInfo();

	UFUNCTION()
	void ThrowAwaySelectedItem();

	UFUNCTION()
	void UseItem();
};
