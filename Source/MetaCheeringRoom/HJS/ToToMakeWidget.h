// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ToToMakeWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UToToMakeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;

	void SetWidgetSwitcher(int32 value);

	// ToToMake -------------------------

	UPROPERTY(EditAnywhere)
	int32 MAX_TEXT_LEN = 15;
	UPROPERTY(EditAnywhere)
	int32 MAX_SELECT_LEN = 10;
	UPROPERTY(meta=(BindWidget))
	class UWidgetSwitcher* WS_MakeToTo;

	UPROPERTY(meta=(BindWidget))
	class UEditableTextBox* ET_Select1;
	UFUNCTION()
	void OnSelect1Changed(const FText& Text);
	UPROPERTY(meta=(BindWidget))
	class UEditableTextBox* ET_Select2;
	UFUNCTION()
	void OnSelect2Changed(const FText& Text);
	UPROPERTY(meta=(BindWidget))
	class UEditableTextBox* ET_BettingName;
	UFUNCTION()
	void OnBettingNameChanged(const FText& Text);

	void CheckCanMakeToTo();

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_SelectLimit1;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_SelectLimit2;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_BettingLimit;

	UPROPERTY(meta=(BindWidget))
	class USpinBox* SPB_TimeLimit;

	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_Cancel;
	UFUNCTION()
	void OnClickCancelBtn();

	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_StartBetting;
	UFUNCTION()
	void OnClickStartBtn();

	//  ---------------------------------------------

	// ToTo Result ---------------------------------

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_BettingName;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_Result1;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_Result2;

	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_CancelResult;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_Adjust;
	UFUNCTION()
	void OnClickAdjustBtn();
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_ResultSelect1;
	UFUNCTION()
	void OnClickResult1Btn();
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_ResultSelect2;
	UFUNCTION()
	void OnClickResult2Btn();
	int32 SelectResult = -1;

	// ---------------------------- 

	bool bOpen = true;

	void InitMakeWidget();

};
