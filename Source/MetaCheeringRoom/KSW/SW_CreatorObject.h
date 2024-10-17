// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SW_CreatorObject.generated.h"

UCLASS()
class METACHEERINGROOM_API ASW_CreatorObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASW_CreatorObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnSelected(bool isSelected);

	void DoDestroy();
	// º¯È¯

	struct FCreatorObjectData* CreatingObjectData;
};
