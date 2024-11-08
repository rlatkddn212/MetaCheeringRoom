// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CreatorObject/SW_CreatorObject.h"
#include "CreatorStorageSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FCreatorObjectData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CObjectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CObjectId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemIcon = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ASW_CreatorObject> ItemClass;
};

USTRUCT(BlueprintType)
struct FCreatorMapMetaData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CreatorName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CreatorMapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FileName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ThumbnailFileName;

	// 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDateTime CreatedTime;
};

USTRUCT(BlueprintType)
struct FCreatorFBXData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CObjectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CObjectId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemIcon = nullptr;

};

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UCreatorStorageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	UCreatorStorageSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void SetCreatorName(const FString& NewCreatorName);

	UFUNCTION(BlueprintCallable)
	FString GetCreatorName() const;

	FCreatorObjectData* GetCreatorObjectData(int32 objectType, int32 objectId);
	TMap<int32, FCreatorObjectData*> GetCreatorObjects(int32 objectType);
	TArray<FCreatorMapMetaData*> GetCreatorMapMetaDatas() { return CreatorMapMetaDatas; }

	FString LoadCreatorMap(FString FilePath);
	bool SaveCreatorMap(FString JsonStr, FString MapName);

	void LoadMetaData();
	void SaveMetaData();
	void AddMetaData(FCreatorMapMetaData* MetaData);
	void RemoveMetaData(int32 idx);

private:
	
	UPROPERTY(EditDefaultsOnly)
	class UDataTable* ItemDataTable;
	// 크리에이터 툴에 사용할 수 있는 오브젝트
	TMap<int32, TMap<int32, FCreatorObjectData*>> CreatorObjectMaps;
	
	TArray<FCreatorMapMetaData*> CreatorMapMetaDatas;
	FString MetaDataFile = "CreatorMapMetaDataList.json";

	TMap<int32, FCreatorFBXData*> CreatorFBXDatas;
	FString FBXDataFile = "CreatorFBXDataList.json";
};
