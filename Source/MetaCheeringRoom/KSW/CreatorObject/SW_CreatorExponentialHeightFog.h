// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KSW/CreatorObject/SW_CreatorObject.h"
#include "SW_CreatorExponentialHeightFog.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API ASW_CreatorExponentialHeightFog : public ASW_CreatorObject
{
	GENERATED_BODY()
public:
	ASW_CreatorExponentialHeightFog();

	virtual void BeginPlay() override;

	virtual void OnChangeProperty(int32 id, UCreatorPropertyBase* CreatorProperty);
	virtual TMap<int32, UCreatorPropertyBase*> GetPropertyMap() override;

	virtual void RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const override;
	virtual void SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject) override;

	class UExponentialHeightFogComponent* ExponentialHeightFogComp;
	float FogDensity;
	float FogHeightFalloff;
	FLinearColor FogInscatteringLuminance;
	float DirectionalInscatteringExponent;
	float DirectionalInscatteringStartDistance;
	bool bEnableVolumetricFog;
	float VolumetricFogScatteringDistribution;
	float VolumetricFogExtinctionScale;
};
