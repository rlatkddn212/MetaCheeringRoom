// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KSW/UI/SW_CreateCreatorSlotWidget.h"
#include "JS_HostSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UJS_HostSlotWidget : public USW_CreateCreatorSlotWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetupInfo(struct FCreatorMapMetaData* metaData, class UJS_SessionJoinWidget* parent, int32 idx);

	UFUNCTION()
	void OnClickHostBtn();

public:
	
	UPROPERTY()
	UJS_SessionJoinWidget* SParent;

	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_HostMap;
};
