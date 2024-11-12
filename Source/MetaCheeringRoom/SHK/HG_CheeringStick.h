// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SHK/HG_EquipItem.h"
#include "HG_CheeringStick.generated.h"

/**
 * 
 */


UCLASS()
class METACHEERINGROOM_API AHG_CheeringStick : public AHG_EquipItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHG_CheeringStick();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame

	virtual void Tick(float DeltaTime) override;

	virtual void Equiped(APawn* Value) override;

	void ChangeColor(FLinearColor Value);

	void ChangeIntensity(float Value);

	void ApplyChange(FLinearColor Color,bool Bling,float Intensity);

	void BlingBling(float DeltaSecond);

	float MinIntensity = 10.0f;
	float MaxIntensity = 100.0f;

	float CurrentIntensity = 40.0f;

	bool bBling = false;

	bool bToggle = false;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UHG_HUD> HeadUpClass;

	UHG_HUD* HUD;

	class UMaterialInstanceDynamic* DynamicMaterial;
};
