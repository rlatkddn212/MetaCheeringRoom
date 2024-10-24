// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SW_CreatorGameBase.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API ASW_CreatorGameBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASW_CreatorGameBase();

	virtual void BeginPlay() override;
};
