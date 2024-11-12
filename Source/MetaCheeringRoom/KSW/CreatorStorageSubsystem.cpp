// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorStorageSubsystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Misc/FileHelper.h"
#include "Engine/Texture2D.h"
#include "Engine/Texture.h"
#include "Templates/SharedPointer.h"
#include "../Util/UtilBlueprintFunctionLibrary.h"

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
	TArray<FCreatorObjectData*> Objects;

	ItemDataTable->GetAllRows(TEXT(""), Objects);
	for (auto* Row : Objects)
	{
		if (!CreatorObjectMaps.Contains(Row->CObjectType))
		{
			CreatorObjectMaps.Add(Row->CObjectType, TMap<int32, FCreatorObjectData*>());
		}

		CreatorObjectMaps[Row->CObjectType].Add(Row->CObjectId, Row);
	}

	// CreatorMapMetaDatas
	LoadMetaData();
}

void UCreatorStorageSubsystem::Deinitialize()
{
	CreatorObjectMaps.Empty();
}

void UCreatorStorageSubsystem::SetCreatorName(const FString& NewCreatorName)
{

}

FString UCreatorStorageSubsystem::GetCreatorName() const
{
	return "";
}

FCreatorObjectData* UCreatorStorageSubsystem::GetCreatorObjectData(int32 objectType, int32 objectId)
{
	if (CreatorObjectMaps.Contains(objectType))
	{
		if (CreatorObjectMaps[objectType].Contains(objectId))
		{
			return CreatorObjectMaps[objectType][objectId];
		}
	}

	return nullptr;
}

TMap<int32, FCreatorObjectData*> UCreatorStorageSubsystem::GetCreatorObjects(int32 objectType)
{
	if (CreatorObjectMaps.Contains(objectType))
	{
		return CreatorObjectMaps[objectType];
	}

	return TMap<int32, FCreatorObjectData*>();
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
	FString FileName = TEXT("CreatorMap") + FDateTime::Now().ToString() + TEXT(".json");
	// 날짜 추가
	FString FilePath = FPaths::ProjectSavedDir() + TEXT("/CreatorMap/") + FileName;

	// 파일이 이미 있는지 검사
	if (FPaths::FileExists(FilePath))
	{
		// TODO : 파일 생성 실패 팝업
		UE_LOG(LogTemp, Error, TEXT("Failed to save CreatorMap to %s"), *FilePath);
		return false;
	}
	FString ThumbnailFilename = MapName + "_" + FGuid::NewGuid().ToString();
	UUtilBlueprintFunctionLibrary::CaptureScreenshot(ThumbnailFilename, L"");
	if (FFileHelper::SaveStringToFile(JsonStr, *FilePath))
	{
		UE_LOG(LogTemp, Log, TEXT("CreatorMap saved to %s"), *FilePath);

		FCreatorMapMetaData* metaData = new FCreatorMapMetaData();
		metaData->CreatorMapName = MapName;
		metaData->FileName = FileName;
		metaData->ThumbnailFileName = ThumbnailFilename;
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

	FString MetaDataFilePath = FPaths::ProjectSavedDir() + TEXT("/CreatorMap/") + MetaDataFile;
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

	FString MetaDataFilePath = FPaths::ProjectSavedDir() + TEXT("/CreatorMap/") + MetaDataFile;
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
	FString FilePath = FPaths::ProjectSavedDir() + TEXT("/CreatorMap/") + CreatorMapMetaDatas[idx]->FileName;
	if (FPaths::FileExists(FilePath))
	{
		FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*FilePath);
	}

	CreatorMapMetaDatas.RemoveAt(idx);
	SaveMetaData();
}

