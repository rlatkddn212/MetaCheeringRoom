// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SW_PropertyEnumWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_PropertyEnumWidget : public UUserWidget
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
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UComboBoxString* ComboValueString;

	int32 PropertyId;
	class UCreatorEnumProperty* EnumProperty;

	void SetPropertyValue(int32 id, class UCreatorEnumProperty* Property);
	
	UFUNCTION()
	void OnEnumChanged(FString SelectedItem, ESelectInfo::Type SelectionTyp);

	void SetInspectorWidget(class USW_CreatorInspectorWidget* InInspectorWidget) { InspectorWidget = InInspectorWidget; }

	// 인스펙터 저장
	class USW_CreatorInspectorWidget* InspectorWidget;
};
