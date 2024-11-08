// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KSW/CreatorObject/SW_CreatorLight.h"
#include "SW_CreatorDirectionalLight.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API ASW_CreatorDirectionalLight : public ASW_CreatorLight
{
	GENERATED_BODY()
	
public:
	ASW_CreatorDirectionalLight();

	virtual void BeginPlay() override;

	// DirectionalLight
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDirectionalLightComponent* LightComp;
};
