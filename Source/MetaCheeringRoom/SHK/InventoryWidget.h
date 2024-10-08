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


	UPROPERTY()
	class UWrapBox* WB_SlotList;

	void InitInventoryUI();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UHG_SlotWidget> SlotWidgetFactory;
};
