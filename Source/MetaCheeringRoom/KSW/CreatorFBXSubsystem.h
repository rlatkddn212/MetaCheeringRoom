// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CreatorFBXSubsystem.generated.h"


USTRUCT(BlueprintType)
struct FCreatorFBXMetaData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FileName;
};

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UCreatorFBXSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	AActor* OpenFBX(FString FilePath);

	void LoadMetaData();
	void SaveMetaData();
	// FBX ÆÄÀÏ
	void AddMetaData(FCreatorFBXMetaData* MetaData);
	void RemoveMetaData(int32 idx);

	AActor* CreateActorFromFBX(const FString& FileName, const FVector& Location, const FRotator& Rotation, const FVector& Scale);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCreatorFBXMetaData> MetaData;
};
