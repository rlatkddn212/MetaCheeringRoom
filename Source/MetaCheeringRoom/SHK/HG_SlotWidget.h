// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

	UFUNCTION()
	void OnButtonClicked();

	void SetItemIcon(UTexture2D* Icon);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AHG_ItemBase> ItemClass;
};
