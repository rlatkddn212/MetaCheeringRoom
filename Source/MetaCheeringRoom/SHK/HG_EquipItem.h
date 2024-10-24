// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HG_ItemBase.h"
#include "HG_EquipItem.generated.h"

UCLASS()
class METACHEERINGROOM_API AHG_EquipItem : public AHG_ItemBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHG_EquipItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame

	virtual void Tick(float DeltaTime) override;

	virtual void Equiped(APawn* Value);

	UPROPERTY(EditDefaultsOnly)
	FString Name;
};
