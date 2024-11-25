// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KSW/CreatorObject/SW_CreatorObject.h"
#include "SW_CreatorPostProcessVolume.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API ASW_CreatorPostProcessVolume : public ASW_CreatorObject
{
	GENERATED_BODY()
	ASW_CreatorPostProcessVolume();

	virtual void BeginPlay() override;

	virtual void OnChangeProperty(int32 id, UCreatorPropertyBase* CreatorProperty);
	virtual TMap<int32, UCreatorPropertyBase*> GetPropertyMap() override;

	virtual void RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const override;
	virtual void SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject) override;
	UPROPERTY()
	class APostProcessVolume* PostProcessActor;
	
	bool bOverride_BloomMethod = true;
	TEnumAsByte<enum EBloomMethod> BloomMethod;

	bool bOverride_BloomIntensity = true;
	float BloomIntensity;

	bool bOverride_BloomThreshold = true;
	float BloomThreshold;

	bool bOverride_AutoExposureMethod = true;
	TEnumAsByte<enum EAutoExposureMethod> AutoExposureMethod;

	bool bOverride_AutoExposureMinBrightness = true;
	float AutoExposureMinBrightness;

	bool bOverride_AutoExposureMaxBrightness = true;
	float AutoExposureMaxBrightness;

	bool bOverride_AutoExposureSpeedUp = true;
	float AutoExposureSpeedUp;

	bool bOverride_AutoExposureSpeedDown = true;
	float AutoExposureSpeedDown;

	bool bOverride_LensFlareIntensity = true;
	float LensFlareIntensity;

	bool bOverride_LensFlareTint = true;
	FLinearColor LensFlareTint;

	bool bOverride_LensFlareBokehSize = true;
	float LensFlareBokehSize;

	bool bOverride_LensFlareThreshold = true;
	float LensFlareThreshold;
};
