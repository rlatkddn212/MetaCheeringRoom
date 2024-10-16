// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_GameInstance.h"
#include "Engine/DataTable.h"

UHG_GameInstance::UHG_GameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> TempDateTable(TEXT("/Script/Engine.DataTable'/Game/SHK/BP/DT_Item.DT_Item'"));

	if (TempDateTable.Succeeded())
	{
		ItemDataTable = TempDateTable.Object;
	}
}
