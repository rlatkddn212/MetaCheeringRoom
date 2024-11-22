// Fill out your copyright notice in the Description page of Project Settings.
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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Tick(float DeltaTime) override;

	virtual void Equiped(APawn* Value) override;

	void ChangeColor(FLinearColor Value);

	void ChangeIntensity(float Value);

	UFUNCTION(BlueprintCallable)
	void ApplyChange(FLinearColor Color,bool Bling,float Intensity);
	
	UFUNCTION(Server,Reliable)
	void ServerRPC_ApplyChange(FLinearColor Color, bool Bling, float Intensity);

	UFUNCTION(NetMulticast,Reliable)
	void Multicast_ApplyChange(FLinearColor Color, bool Bling, float Intensity);

	void BlingBling(float DeltaSecond);

	float MinIntensity = 10.0f;
	float MaxIntensity = 100.0f;

	float CurrentIntensity = 40.0f;

	bool bBling = false;

	bool bToggle = false;

	float time = 0.1;

	class UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(ReplicatedUsing = OnRep_ChangeMatColor)
	FLinearColor MatColor;

	UFUNCTION()
	void OnRep_ChangeMatColor();
};
