// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_PlayerInventoryComponent.h"
#include "HG_ItemBase.h"
#include "HG_GameInstance.h"
#include "HG_Player.h"
#include "HG_HUD.h"

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

	CompOwner = Cast<AHG_Player>(GetOwner());

}


// Called every frame
void UHG_PlayerInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UHG_PlayerInventoryComponent::AddtoInventory(FItemData Item, int32 Quantity)
{
	InitItemData(Item);

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
	if (CompOwner->GI)
	{
		CompOwner->GI->CurrentInventory = Inventory;
	}

	if (CompOwner->HUD)
	{
		CompOwner->HUD->PlayInventoryAnimation();
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
	CompOwner->GI->CurrentInventory = Inventory;
}


void UHG_PlayerInventoryComponent::InitItemData(FItemData& ItemData)
{
	TArray<FItemData*> AllRows;
	if (CompOwner->GI)
	{
		CompOwner->GI->ItemDataTable->GetAllRows(TEXT(""), AllRows);
		for (auto Row : AllRows)
		{
			if (Row->ItemName == ItemData.ItemName)
			{
				ItemData.ItemClass = Row->ItemClass;
				ItemData.ItemIcon = Row->ItemIcon;
				ItemData.ItemName = Row->ItemName;
				ItemData.ItemPrice = Row->ItemPrice;
				ItemData.ItemCategory = Row->ItemCategory;
				ItemData.Woman_Montage = Row->Woman_Montage;
				ItemData.Man_Montage = Row->Man_Montage;
				ItemData.Sound = Row->Sound;
				break;
			}
		}
	}
}
