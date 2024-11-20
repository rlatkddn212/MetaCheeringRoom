// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JS_ChatNode.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UJS_ChatNode : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_ID;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_Chat;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* ShowChat;

	void SetRandomColorText();

	TArray<FColor> Colors = {
		FColor(128, 192, 192),
		FColor(192, 192, 192),
		FColor(32, 32, 32), 
		FColor(128, 128, 128),
		FColor(96, 160, 160), 
		FColor(96, 64, 224), 
		FColor(224, 224, 224), 
		FColor(64, 64, 64), 
		FColor(32, 32, 64),
		FColor(192, 224, 224),
		FColor(160, 160, 160),
		FColor(140, 190, 140),
		FColor(210, 119, 161),
		FColor(166, 210, 147),
	};
};
