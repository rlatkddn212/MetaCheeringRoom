// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SW_CreatorObject.h"
#include "CreatorStorageSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FCreatorObjectData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemIcon = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ASW_CreatorObject> ItemClass;
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

	FCreatorObjectData* GetCreatorObjectData(int idx);
	TArray<FCreatorObjectData*> GetCreatorObjects() { return CreatorObjects; }
private:
	// Json형태로 저장
	FString LoadCreatorMap();
	void SaveCreatorMap(FString str);

	
	UPROPERTY(EditDefaultsOnly)
	class UDataTable* ItemDataTable;
	// 크리에이터 툴에 사용할 수 있는 오브젝트
	TArray<FCreatorObjectData*> CreatorObjects;
};
