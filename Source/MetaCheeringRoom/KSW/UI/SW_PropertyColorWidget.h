// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SW_PropertyColorWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_PropertyColorWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	// Bind
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* NameText;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ColorImageButton;

	int32 PropertyId;
	UPROPERTY()
	class UCreatorColorProperty* ColorProperty;

	void SetPropertyValue(int32 id, class UCreatorColorProperty* Property);

	void SetInspectorWidget(class USW_CreatorInspectorWidget* InInspectorWidget);
	
	UFUNCTION()
	void OnColorChanged(FLinearColor Color);
	
	UFUNCTION()
	void OnColorButtonClicked();

	// 인스펙터 저장
	UPROPERTY()
	class USW_CreatorInspectorWidget* InspectorWidget;
};
