// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_PlayerInventoryComponent.h"
#include "HG_ItemBase.h"

// Sets default values for this component's properties
UHG_PlayerInventoryComponent::UHG_PlayerInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableObj(TEXT("/Script/Engine.DataTable'/Game/SHK/BP/DT_Item.DT_Item'"));
	if (DataTableObj.Succeeded())
	{
		ItemDataTable = DataTableObj.Object;
	}

	InventorySize = 30;
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

void UHG_PlayerInventoryComponent::AddtoInventory(FItemData Item, int32 Quantity)
{
	if (Inventory.Num() <= InventorySize)
	{

		if (FindSlot(Item.ItemName) != -1)
		{
			Inventory[FindSlot(Item.ItemName)].Quantity += Quantity;
		}
		else
		{
			FSlotStruct Temp;
			Temp.ItemInfo = Item;
			Temp.Quantity += Quantity;
			Inventory.Add(Temp);
		}
	}
	for (int i = 0; i < Inventory.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory[%d] = %s,%d"), i,*Inventory[i].ItemInfo.ItemName,Inventory[i].Quantity);
	}
}

int32 UHG_PlayerInventoryComponent::FindSlot(FString ItemName)
{
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i].ItemInfo.ItemName == ItemName)
		{
			return i;
		}
	}
	return -1;
}

void UHG_PlayerInventoryComponent::RemoveFromInventory(FItemData Item, int32 Quantity)
{
	if (FindSlot(Item.ItemName) != -1)
	{
		Inventory[FindSlot(Item.ItemName)].Quantity = FMath::Clamp(Inventory[FindSlot(Item.ItemName)].Quantity - Quantity, 0, InventorySize);
		if (Inventory[FindSlot(Item.ItemName)].Quantity <= 0)
		{ 
			Inventory.RemoveAt(FindSlot(Item.ItemName));
		}
	}
	for (int i = 0; i < Inventory.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory[%d] = %s,%d"), i, *Inventory[i].ItemInfo.ItemName, Inventory[i].Quantity);
	}
}



