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

	for (auto* Row : CreatorObjects)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCreatorStorageSubsystem"));
		UE_LOG(LogTemp, Warning, TEXT("ItemName : %s"), *Row->ItemName);
	}

	// CreatorMapMetaDatas
	LoadMetaData();
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

bool UCreatorStorageSubsystem::SaveCreatorMap(FString JsonStr, FString MapName)
{
	// 날짜 추가
	//FString path = FPaths::ProjectContentDir() + TEXT("CreatorMap.json");
	FString FilePath = FPaths::ProjectContentDir() + TEXT("CreatorMap") + FDateTime::Now().ToString() + TEXT(".json");

	 if (FFileHelper::SaveStringToFile(JsonStr, *FilePath))
	 {
		 UE_LOG(LogTemp, Log, TEXT("CreatorMap saved to %s"), *FilePath);

		 FCreatorMapMetaData* metaData = new FCreatorMapMetaData();
		 metaData->CreatorMapName = MapName;
		 metaData->FileName = FilePath;
		 metaData->ThumbnailFileName = "ThumbnailFileName";
		 metaData->CreatedTime = FDateTime::Now();
		 AddMetaData(metaData);

		 return true;
	 }

	 UE_LOG(LogTemp, Error, TEXT("Failed to save CreatorMap to %s"), *FilePath);

	 return false;
}

void UCreatorStorageSubsystem::LoadMetaData()
{
	FString JsonStr;

	FString MetaDataFilePath = FPaths::ProjectContentDir() + MetaDataFile;
	if (FFileHelper::LoadFileToString(JsonStr, *MetaDataFilePath))
	{
		UE_LOG(LogTemp, Log, TEXT("CreatorMap loaded from %s"), *MetaDataFilePath);
	}

	CreatorMapMetaDatas.Empty();

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonStr);

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		TArray<TSharedPtr<FJsonValue>> JsonArray = JsonObject->GetArrayField("CreatorMapMetaDatas");

		for (TSharedPtr<FJsonValue> JsonValue : JsonArray)
		{
			TSharedPtr<FJsonObject> JsonObj = JsonValue->AsObject();
			FCreatorMapMetaData* MetaData = new FCreatorMapMetaData();
			MetaData->CreatorMapName = JsonObj->GetStringField("CreatorMapName");
			MetaData->FileName = JsonObj->GetStringField("FileName");
			MetaData->ThumbnailFileName = JsonObj->GetStringField("ThumbnailFileName");
			MetaData->CreatedTime = FDateTime::FromUnixTimestamp(JsonObj->GetNumberField("CreatedTime"));
			CreatorMapMetaDatas.Add(MetaData);
		}
	}
}

void UCreatorStorageSubsystem::SaveMetaData()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TArray<TSharedPtr<FJsonValue>> JsonArray;

	for (FCreatorMapMetaData* MetaData : CreatorMapMetaDatas)
	{
		TSharedPtr<FJsonObject> JsonObj = MakeShareable(new FJsonObject);
		JsonObj->SetStringField("CreatorMapName", MetaData->CreatorMapName);
		JsonObj->SetStringField("FileName", MetaData->FileName);
		JsonObj->SetStringField("ThumbnailFileName", MetaData->ThumbnailFileName);
		JsonObj->SetNumberField("CreatedTime", MetaData->CreatedTime.ToUnixTimestamp());
		JsonArray.Add(MakeShareable(new FJsonValueObject(JsonObj)));
	}

	JsonObject->SetArrayField("CreatorMapMetaDatas", JsonArray);

	FString JsonStr;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&JsonStr);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);
	FString MetaDataFilePath = FPaths::ProjectContentDir() + MetaDataFile;
	if (FFileHelper::SaveStringToFile(JsonStr, *MetaDataFilePath))
	{
		UE_LOG(LogTemp, Log, TEXT("CreatorMap saved to %s"), *MetaDataFilePath);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save CreatorMap to %s"), *MetaDataFilePath);
	}
}

void UCreatorStorageSubsystem::AddMetaData(FCreatorMapMetaData* MetaData)
{
	CreatorMapMetaDatas.Add(MetaData);
	SaveMetaData();
}

void UCreatorStorageSubsystem::RemoveMetaData(int32 idx)
{
	FString FilePath = FPaths::ProjectContentDir() + CreatorMapMetaDatas[idx]->FileName;
	if (FPaths::FileExists(FilePath))
	{
		FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*FilePath);
	}

	CreatorMapMetaDatas.RemoveAt(idx);
	SaveMetaData();
}
