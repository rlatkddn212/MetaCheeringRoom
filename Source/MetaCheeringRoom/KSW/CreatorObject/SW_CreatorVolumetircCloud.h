// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KSW/CreatorObject/SW_CreatorObject.h"
#include "SW_CreatorVolumetircCloud.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API ASW_CreatorVolumetircCloud : public ASW_CreatorObject
{
	GENERATED_BODY()
	
public:
	ASW_CreatorVolumetircCloud();

	virtual void BeginPlay() override;

	virtual void OnChangeProperty(int32 id, UCreatorPropertyBase* CreatorProperty);
	virtual TMap<int32, UCreatorPropertyBase*> GetPropertyMap() override;

	virtual void RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const override;
	virtual void SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject) override;

	class UVolumetricCloudComponent* VolumetricCloudComp;

	float LayerBottomAltitude;
	float LayerHeight;
	float TracingStartMaxDistance;
	float TracingStartDistanceFromCamera;
	float TracingMaxDistance;

	bool bUsePerSampleAtmosphericLightTransmittance;
	float SkyLightCloudBottomOcclusion;
	float ViewSampleCountScale;
	float ReflectionViewSampleCountScaleValue;
	float ShadowViewSampleCountScale;
	float ShadowReflectionViewSampleCountScaleValue;
	float ShadowTracingDistance;
	float StopTracingTransmittanceThreshold;

	float AerialPespectiveRayleighScatteringStartDistance;
	float AerialPespectiveRayleighScatteringFadeDistance;
	float AerialPespectiveMieScatteringStartDistance;
	float AerialPespectiveMieScatteringFadeDistance;
};
