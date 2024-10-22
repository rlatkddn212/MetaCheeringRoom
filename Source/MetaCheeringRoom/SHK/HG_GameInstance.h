// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HG_GameInstance.generated.h"

/**
 *
 */
UCLASS()
class METACHEERINGROOM_API UHG_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UHG_GameInstance();

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* ItemDataTable;

	bool IsValidItem(FString ItemName);
};