// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HG_TeemoDummy.generated.h"

UCLASS()
class METACHEERINGROOM_API AHG_TeemoDummy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHG_TeemoDummy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;

	float Speed = 450.0f;
	FVector Direction;

	UPROPERTY(EditAnywhere)
	float CoolDown = 3.0f;
};
