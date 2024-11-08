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

	// SpotLight
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USpotLightComponent* LightComp;
};
