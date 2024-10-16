// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SHK/HG_ItemBase.h"
#include "HG_FireCracker.generated.h"

/**
 *
 */
UCLASS()
class METACHEERINGROOM_API AHG_FireCracker : public AHG_ItemBase
{
	GENERATED_BODY()
protected:
public:
	virtual void BeginPlay() override;
	virtual void Use() override;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	class UParticleSystem* FireCrackerEffect;
};
