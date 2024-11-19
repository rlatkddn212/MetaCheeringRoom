// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlotStruct.h"
#include "HG_SlotWidget.generated.h"

/**
 *
 */
UCLASS()
class METACHEERINGROOM_API UHG_SlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly)
	class UButton* Button_InventorySlot;

	UPROPERTY(EditDefaultsOnly)
	class UImage* Img_ItemIcon;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* TB_ItemName;

	void SetItemName();

	UFUNCTION()
	void OnButtonClicked();
	
	UFUNCTION()
	void OnButtonHovered();
	
	UFUNCTION()
	void OnButtonUnHovered();

	void SetItemIcon();


	void InitSlot(FSlotStruct p_Slot);

	void SetOwner(class UInventoryWidget* p_Owner);

	FSlotStruct SlotInfo;

	UPROPERTY(EditDefaultsOnly)
	UInventoryWidget* Owner;

	bool bSelected = false;

	FButtonStyle NormalStyle;

	int32 MyIndex;
};
