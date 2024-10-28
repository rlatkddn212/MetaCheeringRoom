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
	UPROPERTY(meta=(BindWidget))
	class UBorder* BD_Toast;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* SaveButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* QuitButton;
	
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* ToastAnim;

	UFUNCTION()
	void SaveCreatorMap();
	UFUNCTION()
	void Quit();
};
