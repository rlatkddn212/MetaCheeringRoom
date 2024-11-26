// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JS_FeverWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UJS_FeverWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	
	UPROPERTY(meta=(BindWidget))
	class UImage* IMG_Fever;
	UPROPERTY(meta=(BindWidget))
	class UImage* IMG_T;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_Fever;
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* PB_FeverBar;
	UFUNCTION(BlueprintCallable)
	void SetPercent(float value);

	UFUNCTION(BlueprintCallable)
	float GetPercent();

	void DownPercent(float InDeltaTime);

    FLinearColor CurrentColor;
    float TimeElapsed;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gradient")
    float ColorChangeSpeed = 1.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bFeverStart = false;
	bool bFeverTime = false;
	float FeverEndTime = 5.f;
	float CurrentTime = 0.f;
	void UpdateGradientColor(float InDeltaTime);

	void FeverStart();

	UPROPERTY(EditAnywhere)
	USoundBase* DrumRollSound;
	UPROPERTY(EditAnywhere)
	USoundBase* DrumHitSound;
	UPROPERTY(EditAnywhere)
	USoundBase* FeverSound;
	UPROPERTY(meta=(BindWidget))
	class UImage* IMG_FeverTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gradient")
    float ColorChangeSpeed2 = 0.5f;
	void TextGradientColor(float InDeltaTime);
	float TimeElapsed2 = 0.f;
	FLinearColor CurrenTextColor;

	UPROPERTY(meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* Fever;
	
};
