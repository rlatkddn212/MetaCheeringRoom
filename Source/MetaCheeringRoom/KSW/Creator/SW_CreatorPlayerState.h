// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SW_CreatorPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API ASW_CreatorPlayerState : public APlayerState
{
	GENERATED_BODY()
	
	public:

	virtual void BeginPlay() override;
	
	UFUNCTION(Server, Reliable)
	void ServerRPCSetPlayerName(const FString& newName);
};
