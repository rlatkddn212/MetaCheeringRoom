// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HG_ItemBase.h"
#include "HG_EasterEgg.generated.h"

UCLASS()
class METACHEERINGROOM_API AHG_EasterEgg : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHG_EasterEgg();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxCollisionComp;

	float StayTime = 5.0f;

	bool bStaying = false;

	bool bEasterEggComplete = false;
	
	UFUNCTION()
	void OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION()
	void OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	class AHG_Player* OverlapPlayer;

	void EasterEggComplete();

	FItemData ItemData;

	class UHG_GameInstance* GI;

	void InitItemData();

	void SetItemData(FItemData p_ItemData);
};
