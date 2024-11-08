// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KSW/CreatorObject/SW_CreatorObject.h"
#include "SW_CreatorFBX.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API ASW_CreatorFBX : public ASW_CreatorObject
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ASW_CreatorFBX();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnSelected(bool isSelected) override;

	void SelectChildActor(AActor* actor, bool isSelected);
};
