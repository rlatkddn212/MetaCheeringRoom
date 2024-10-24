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


	UFUNCTION()
	virtual void Use() override;

	UFUNCTION(Server,Reliable)
	void ServerRPCUse();	 
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCUse(APawn* pawn);


	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	class UParticleSystem* FireCrackerEffect;
};
