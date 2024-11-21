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
	float FeverFailTime = 5.f;
	bool bFeverTime = false;
	float FeverEndTime = 5.f;
	void UpdateGradientColor(float InDeltaTime);

	void FeverStart();
};
