// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/Widget.h"
#include "JS_EasingFunctionLib.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UJS_EasingFunctionLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
    // Elastic Ease Out 효과를 적용하는 함수
    UFUNCTION(BlueprintCallable, Category = "UI Animation")
    static float ApplyElasticEaseOut(float Start, float End, float Alpha);
    UFUNCTION(BlueprintCallable, Category = "UI Animation")
    static float ApplyBounceEaseIn(float Start, float End, float Alpha);
    UFUNCTION(BlueprintCallable, Category = "UI Animation")
    static float ElasticEaseOut(float Alpha);
    UFUNCTION(BlueprintCallable, Category = "UI Animation")
    static float BounceEaseIn(float Alpha);
    UFUNCTION(BlueprintCallable, Category = "UI Animation")
    static float BounceEaseOut(float Alpha);
    UFUNCTION(BlueprintCallable, Category = "UI Ease")
    static float FadeInEase(float Alpha);
    UFUNCTION(BlueprintCallable, Category = "UI Ease")
    static float FadeOutEase(float Alpha);
};
