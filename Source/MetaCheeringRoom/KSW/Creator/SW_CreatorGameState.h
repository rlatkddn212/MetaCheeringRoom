// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SW_CreatorGameState.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API ASW_CreatorGameState : public AGameStateBase
{
	GENERATED_BODY()
	
	public:

	virtual void BeginPlay() override;
};
