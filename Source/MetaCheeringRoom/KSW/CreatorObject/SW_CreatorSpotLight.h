// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KSW/CreatorObject/SW_CreatorLight.h"
#include "SW_CreatorSpotLight.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API ASW_CreatorSpotLight : public ASW_CreatorLight
{
	GENERATED_BODY()
	
public:
	ASW_CreatorSpotLight();

	virtual void OnChangeProperty(int32 id, UCreatorPropertyBase* CreatorProperty);
	virtual TMap<int32, UCreatorPropertyBase*> GetPropertyMap() override;

	virtual void RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const override;
	virtual void SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject) override;

	// SpotLight
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USpotLightComponent* LightComp;

	FLinearColor LightColor;
	float LightIntensity;
	float LightOuterConeAngle;
	float LightInnerConeAngle;
};
