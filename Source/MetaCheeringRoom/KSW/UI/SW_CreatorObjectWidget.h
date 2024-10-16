// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SW_CreatorObjectWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_CreatorObjectWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USW_CreatorObjectSlotWidget> SlotFactory;

	/*UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TArray<class USW_CreatorObjectSlotWidget*> SlotWidgets;*/

	// ObjectScrollBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* ObjectScrollBox;

};
