// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KSW/CreatorObject/SW_CreatorLight.h"
#include "SW_CreatorRectLight.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API ASW_CreatorRectLight : public ASW_CreatorLight
{
	GENERATED_BODY()

public:
	ASW_CreatorRectLight();

	// RectLight
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class URectLightComponent* LightComp;
};
