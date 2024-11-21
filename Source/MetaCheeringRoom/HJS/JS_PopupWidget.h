// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JS_PopupWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UJS_PopupWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_1;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_2;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_3;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_PopupCancel;
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* ShowWidget;

	UFUNCTION()
	void OpenRoomUI();

	UFUNCTION()
	void ClosePopupUI();

	UFUNCTION(BlueprintCallable)
	void Init();

};
