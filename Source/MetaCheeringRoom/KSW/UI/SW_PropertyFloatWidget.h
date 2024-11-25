// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SW_PropertyFloatWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_PropertyFloatWidget : public UUserWidget
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
	class UEditableText* ValueText;

	int32 PropertyId;
	UPROPERTY()
	class UCreatorFloatProperty* FloatProperty;

	void SetPropertyValue(int32 id, class UCreatorFloatProperty* Property);
	
	UFUNCTION()
	void OnFloatChanged(const FText& Text, ETextCommit::Type CommitMethod);

	void SetInspectorWidget(class USW_CreatorInspectorWidget* InInspectorWidget) { InspectorWidget = InInspectorWidget; }

	// �ν����� ����
	UPROPERTY()
	class USW_CreatorInspectorWidget* InspectorWidget;
};
