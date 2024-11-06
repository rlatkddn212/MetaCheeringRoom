// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JS_SumarryRequestWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UJS_SumarryRequestWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
	UPROPERTY(meta=(BindWidget))
	class UComboBoxString* CB_Summary;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_Summary;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_Cancel;
	UPROPERTY(meta=(BindWidget))
	class UBorder* BD_Summary;

	UPROPERTY()
	class AJS_Screen* Screen;

	UFUNCTION()
	void OnClickSummaryBtn();
	UFUNCTION()
	void OnClickCancelBtn();

	bool bIsAnimating = false;
	float AnimationAlpha = 0.f;
	UPROPERTY(EditAnywhere)
	float AnimationDuration = 0.8f;
	UPROPERTY(EditAnywhere)
	float StartPosition;
	UPROPERTY(EditAnywhere)
	float TargetOffset = 200.f;
	void PlayShowAnimation();
	void OnAnimation(float DeltaTime);

};
