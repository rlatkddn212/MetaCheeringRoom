// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_GameInstance.h"
#include "Engine/DataTable.h"
#include "HG_ItemBase.h"

UHG_GameInstance::UHG_GameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> TempDateTable(TEXT("/Script/Engine.DataTable'/Game/SHK/BP/DT_Item.DT_Item'"));

	if (TempDateTable.Succeeded())
	{
		ItemDataTable = TempDateTable.Object;
	}
}

bool UHG_GameInstance::IsValidItem(FString ItemName)
{
	TArray<FItemData*> AllRows;
	UE_LOG(LogTemp, Warning, TEXT("IsValidItem"));
	if (ItemDataTable)
	{
		ItemDataTable->GetAllRows(TEXT(""), AllRows);
	}
	for (auto Row : AllRows)
	{
		if (Row->ItemName == ItemName)
		{
			return true;
		}
	}
	return false;
}

FItemData UHG_GameInstance::FindItemData(FString ItemName)
{
	TArray<FItemData*> AllRows;

	ItemDataTable->GetAllRows(TEXT(""), AllRows);

	for (auto Row : AllRows)
	{
		if (Row->ItemName == ItemName)
		{
			return *Row;
		}
	}
	return FItemData();
}
