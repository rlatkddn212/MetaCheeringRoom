// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_CoinActor.generated.h"

UCLASS()
class METACHEERINGROOM_API AJS_CoinActor : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AJS_CoinActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* MeshComp;

	UPROPERTY()
	class ACharacter* Player;

	UFUNCTION()
	void ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	bool bWayPoint = false;

};
