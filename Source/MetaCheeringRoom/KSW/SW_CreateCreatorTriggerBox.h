// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SW_CreateCreatorTriggerBox.generated.h"

UCLASS()
class METACHEERINGROOM_API ASW_CreateCreatorTriggerBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASW_CreateCreatorTriggerBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;

	UFUNCTION()
	void ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
