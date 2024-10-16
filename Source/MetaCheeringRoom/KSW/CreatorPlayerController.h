// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CreatorPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API ACreatorPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACreatorPlayerController();
	virtual void BeginPlay() override;
};
