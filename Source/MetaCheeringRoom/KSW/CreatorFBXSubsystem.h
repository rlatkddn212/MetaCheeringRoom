// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HttpFwd.h"
#include "CreatorFBXSubsystem.generated.h"

// ���ε� ���� �������̵�
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUploadSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDownloadSuccess);

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

	AActor* OpenAndCopyFBX(const FString& FilePath, const FString& NewFileName);
	AActor* LoadFBX(FString FileName);

	bool IsFileExist(const FString& FileName);
	
	void AnonymousLogin();
	void OnAnonymousLoginComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void FileUploadToFirebase(const FString& FilePath, const FString& FileName);
	void FileDownloadFromFirebase(const FString& SavePath, const FString& FileName, TFunction<void()> Func);

	void LoadMetaData();
	void SaveMetaData();
	// FBX ����
	void AddMetaData(FCreatorFBXMetaData* MetaData);
	void RemoveMetaData(int32 idx);

	AActor* CreateActorFromFBX(const FString& FileName, const FVector& Location, const FRotator& Rotation, const FVector& Scale);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCreatorFBXMetaData> MetaData;

	// �α��� ����
	FString Token;

	FOnUploadSuccess OnUploadSuccess;
	FOnDownloadSuccess OnDownloadSuccess;
};
