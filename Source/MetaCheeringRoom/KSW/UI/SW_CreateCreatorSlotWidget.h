// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SW_CreateCreatorSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_CreateCreatorSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetupInfo(struct FCreatorMapMetaData* metaData, class USW_CreateCreatorWidget* parent, int32 idx);

	UFUNCTION()
	void OnJoin();

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* IMG_Thumbnail;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TEXT_Creator;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TEXT_Date;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TEXT_MapName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* BTN_Join;

	USW_CreateCreatorWidget* Parent;
	struct FCreatorMapMetaData* MapMetaData;

	int32 slotIdx;
};
