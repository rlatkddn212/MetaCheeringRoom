// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorStorageSubsystem.h"

UCreatorStorageSubsystem::UCreatorStorageSubsystem()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> TempDateTable(TEXT("/Game/Ksw/Data/DT_CreatorObject.DT_CreatorObject"));

	if (TempDateTable.Succeeded())
	{
		ItemDataTable = TempDateTable.Object;
	}
}

void UCreatorStorageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	ItemDataTable->GetAllRows(TEXT(""), CreatorObjects);
	for (auto* Row : CreatorObjects)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCreatorStorageSubsystem"));
		UE_LOG(LogTemp, Warning, TEXT("ItemName : %s"), *Row->ItemName);
	}
}

void UCreatorStorageSubsystem::Deinitialize()
{
	CreatorObjects.Empty();
}

void UCreatorStorageSubsystem::SetCreatorName(const FString& NewCreatorName)
{

}

FString UCreatorStorageSubsystem::GetCreatorName() const
{
	return "";
}

FCreatorObjectData* UCreatorStorageSubsystem::GetCreatorObjectData(int idx)
{
	return CreatorObjects[idx];
}

FString UCreatorStorageSubsystem::LoadCreatorMap(FString FilePath)
{
	FString JsonStr;
	if (FFileHelper::LoadFileToString(JsonStr, *FilePath))
	{
		UE_LOG(LogTemp, Log, TEXT("CreatorMap loaded from %s"), *FilePath);
		return JsonStr;
	}
	UE_LOG(LogTemp, Error, TEXT("Failed to load CreatorMap from %s"), *FilePath);

	return "";

}

bool UCreatorStorageSubsystem::SaveCreatorMap(FString FilePath, FString JsonStr)
{
	 if (FFileHelper::SaveStringToFile(JsonStr, *FilePath))
	 {
		 UE_LOG(LogTemp, Log, TEXT("CreatorMap saved to %s"), *FilePath);
		 return true;
	 }
	 UE_LOG(LogTemp, Error, TEXT("Failed to save CreatorMap to %s"), *FilePath);

	 return false;
}
