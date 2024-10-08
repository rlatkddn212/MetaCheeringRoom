// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HG_ItemBase.h"
#include "SlotStruct.generated.h"
/**
 *
 */
class METACHEERINGROOM_API SlotStruct
{
public:
	SlotStruct();
	~SlotStruct();
};

USTRUCT(Atomic, BlueprintType)
struct FSlotStruct
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
	int32 Qunatity;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AHG_ItemBase> ItemClass;
};
