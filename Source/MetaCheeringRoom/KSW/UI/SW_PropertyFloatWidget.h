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
	// Bind
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* NameText;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UEditableText* ValueText;

	int32 PropertyId;
	class UCreatorFloatProperty* FloatProperty;

	void SetPropertyValue(int32 id, class UCreatorFloatProperty* Property);
	
	UFUNCTION()
	void OnFloatChanged(const FText& Text, ETextCommit::Type CommitMethod);

	void SetInspectorWidget(class USW_CreatorInspectorWidget* InInspectorWidget) { InspectorWidget = InInspectorWidget; }

	// 인스펙터 저장
	class USW_CreatorInspectorWidget* InspectorWidget;
};
