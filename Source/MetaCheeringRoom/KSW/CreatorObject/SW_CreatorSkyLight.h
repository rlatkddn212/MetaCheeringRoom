// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KSW/CreatorObject/SW_CreatorLight.h"
#include "SW_CreatorSkyLight.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API ASW_CreatorSkyLight : public ASW_CreatorLight
{
	GENERATED_BODY()
	public:
	ASW_CreatorSkyLight();
	virtual void BeginPlay() override;
	// SkyLight
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkyLightComponent* LightComp;
};
