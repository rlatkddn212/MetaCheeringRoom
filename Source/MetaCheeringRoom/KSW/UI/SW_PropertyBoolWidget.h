// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SW_PropertyBoolWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_PropertyBoolWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCheckBox* bCheckBox;

	UFUNCTION()
	void OnCheckChanged(bool bIsChecked);

	// Bind
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* NameText;
	
	// 체크박스
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UCheckBox* CheckBoxValue;
	
	int32 PropertyId;
	class UCreatorBoolProperty* BoolProperty;

	void SetPropertyValue(int32 id, class UCreatorBoolProperty* Property);
	
	UFUNCTION()
	void OnBoolChanged(bool bIsChecked);

	void SetInspectorWidget(class USW_CreatorInspectorWidget* InInspectorWidget) { InspectorWidget = InInspectorWidget; }

	// 인스펙터 저장
	class USW_CreatorInspectorWidget* InspectorWidget;
};
