// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_PlayerInventoryComponent.h"
#include "HG_ItemBase.h"

// Sets default values for this component's properties
UHG_PlayerInventoryComponent::UHG_PlayerInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UHG_PlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UHG_PlayerInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UHG_PlayerInventoryComponent::AddtoInventory(FString ItemName, int32 Quantity,AHG_ItemBase* Item)
{
	if (Inventory.Num() <= InventorySize)
	{
		
		if (FindSlot(Item) != -1)
		{
			Inventory[FindSlot(Item)].Qunatity += Quantity;
		}
		else
		{	
			FSlotStruct Temp;
			Temp.ItemClass = Item->GetClass();
			Temp.Qunatity += Quantity;
			Inventory.Add(Temp);
		}
	}
}

int32 UHG_PlayerInventoryComponent::FindSlot(AHG_ItemBase* Item)
{
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i].ItemClass == Item->GetClass())
		{
			return i;
		}
	}
	return -1;
}



