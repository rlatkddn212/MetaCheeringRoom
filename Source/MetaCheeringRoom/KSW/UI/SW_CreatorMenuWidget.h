// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SW_CreatorMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_CreatorMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* SaveButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* QuitButton;

	UFUNCTION()
	void SaveCreatorMap();
	UFUNCTION()
	void Quit();
};
