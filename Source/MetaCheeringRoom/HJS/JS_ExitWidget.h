// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JS_ExitWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UJS_ExitWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_Quit;
	
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_Cancel;

	UFUNCTION()
	void OnClickQuitBtn();

	UFUNCTION()
	void OnClickCancelBtn();
};
