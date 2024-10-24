// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Delegates/DelegateCombinations.h"
#include "JS_GameState.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API AJS_GameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    AJS_GameState();

    virtual void BeginPlay() override;


    UFUNCTION()
    void HandleBettingToTo(int32 point, int32 select, const FString& UserId);

    UPROPERTY()
    class AJS_TotoActor* TotoActor;
	UFUNCTION(Server, Reliable)
	void ServerHandleBettingToTo(int32 point, int32 select, const FString& UserId);
};
