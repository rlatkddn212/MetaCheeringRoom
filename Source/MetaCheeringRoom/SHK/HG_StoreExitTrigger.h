// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HG_StoreExitTrigger.generated.h"

/**
 *
 */
UCLASS()
class METACHEERINGROOM_API AHG_StoreExitTrigger : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AHG_StoreExitTrigger();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxCollisionComp;

	UFUNCTION()
	void OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
