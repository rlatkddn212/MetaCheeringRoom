// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_StadiumWave.generated.h"

UCLASS()
class METACHEERINGROOM_API AJS_StadiumWave : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_StadiumWave();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;

    bool bTurn = false;
    float CurrentRotation = 0.0f;
    
    // 회전 속도 (초당 각도)
    UPROPERTY(EditAnywhere, Category = "Rotation")
    float RotationSpeed = 90.0f; // 90도/초 (4초 동안 360도 회전)

	// 특정 시점을 기준으로 360도 돈다.
	UFUNCTION(BlueprintCallable)
	void TurnStart();
	
	UFUNCTION(NetMulticast,Reliable)
	void MulticastTurnStart();

	// 360도 돌고 나면 멈추기
	void OnTurn(float DeltaTime);
	int32 i =0;
	UFUNCTION()
	void ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};