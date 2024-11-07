// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RLFProgress.generated.h"


UENUM()
enum CalculateCollisionMethod
{
    eNoCollision=0,
    eSimpleCollision,
    eComplexCollision,
};


/**
 * 
 */


UCLASS(BlueprintType, Blueprintable, meta = (ShortTooltip = "RuntimeLoadFbx Async callback after file imported complete."))
class RUNTIMELOADFBX_API URLFProgress : public UObject
{
	GENERATED_BODY()
public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FProgressCal, const FString&,filePath,float, fPosIn01);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFinishCal, const FString&, filePath, AActor*, rltActor);

    UPROPERTY(BlueprintAssignable, Category = "RuntimeLoadFbx", meta = (ShortTooltip = "called after file imported complete"))
        FProgressCal progressCal;
    UPROPERTY(BlueprintAssignable, Category = "RuntimeLoadFbx", meta = (ShortTooltip = "called while importing"))
        FFinishCal finishCal;

    FString filePath;

    void CalProgress(float f);
    void CalFinish(AActor* actor);
};
