// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SlotStruct.h"
#include "HG_PlayerInventoryComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class METACHEERINGROOM_API UHG_PlayerInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHG_PlayerInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly)
	int32 InventorySize;

	TArray<FSlotStruct> Inventory;

	void AddtoInventory(FItemData Item, int32 Quantity);

	int32 FindSlot(FString ItemName);

	void RemoveFromInventory(FItemData Item, int32 Quantity);
	
	UPROPERTY(EditDefaultsOnly)
	class UDataTable* ItemDataTable;

	class AHG_Player* CompOwner;
};
