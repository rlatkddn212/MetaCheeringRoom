// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SHK/HG_ItemBase.h"
#include "HG_TestItem.generated.h"

/**
 *
 */
UCLASS()
class METACHEERINGROOM_API AHG_TestItem : public AHG_ItemBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	virtual void Use() override;
};
