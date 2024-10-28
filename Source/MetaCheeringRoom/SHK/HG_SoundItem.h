// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SHK/HG_ItemBase.h"
#include "HG_SoundItem.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API AHG_SoundItem : public AHG_ItemBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
	virtual void Use() override;

	UPROPERTY(EditDefaultsOnly)
	FString Name;

	UFUNCTION(Server,Reliable)
	void ServerRPC_Use();

	UFUNCTION(NetMulticast,Reliable)
	void MulticastRPC_Use();
};
