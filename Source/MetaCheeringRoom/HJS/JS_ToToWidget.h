// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JS_ToToWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UJS_ToToWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UBorder* BD_Tost;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_ToToTost;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_Select1;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_Select2;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_ToToName;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_TimeLimit;
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_Percent1;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_Percent2;

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* PB_Percent1;
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* PB_Percent2;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_ToToLog;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_PointSelect1;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_PointSelect2;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_OddsSelect1;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_OddsSelect2;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_BettorCount1;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_BettorCount2;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_SoSad;

	UPROPERTY(meta=(BindWidget))
	class UWidgetSwitcher* WS_Betting;

	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_BettingWeight;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_Betting1;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_Betting2;

	UPROPERTY(meta=(BindWidget))
	class UEditableTextBox* ET_BettingWeight1;
	UPROPERTY(meta=(BindWidget))
	class UEditableTextBox* ET_BettingWeight2;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_WeightBetting1;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_WeightBetting2;

	UFUNCTION()
	void SetTotoInfo(const FString& totoName, const FString& select1, const FString& select2,
		int32 totalSelect1 = 0, int32 totalSelect2 = 0, int32 totalBettor1 = 0, int32 totalBettor2 = 0, float totalOdds1 = 1.f, float totalOdds2 = 1.f);
};
