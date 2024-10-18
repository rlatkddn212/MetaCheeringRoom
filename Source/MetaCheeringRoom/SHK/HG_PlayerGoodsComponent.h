// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HG_PlayerGoodsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class METACHEERINGROOM_API UHG_PlayerGoodsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHG_PlayerGoodsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetGold(int32 Value);
	int32 GetGold() const;
	int32 AddGold(int32 Value);
	int32 SubGold(int32 Value);

private:
	int32 Gold;
	int32 MAX_GOLD = 4000;
};
