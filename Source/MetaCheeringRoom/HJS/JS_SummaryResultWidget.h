// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JS_SummaryResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UJS_SummaryResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_Summary;

	UPROPERTY(meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* SummaryActiveAnim;

	UPROPERTY()
	class AJS_Screen* Screen;
	
	UFUNCTION()
	void ShowSummaryData(const FString& Result);

};
