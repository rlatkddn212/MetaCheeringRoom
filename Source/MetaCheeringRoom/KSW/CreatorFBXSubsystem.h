// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HttpFwd.h"
#include "Containers/Queue.h"
#include "Containers/Map.h"
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FBXName;
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

	AActor* OpenAndCopyFBX(const FString& FilePath, const FString& NewFileName, class URLFProgress* ProgressTracker);
	AActor* LoadFBX(FString FileName, class URLFProgress* Progress);

	bool IsFileExist(const FString& FileName);
	
	void AnonymousLogin();
	void OnAnonymousLoginComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void FileUploadToFirebase(const FString& FilePath, const FString& FileName);
	void FileDownloadFromFirebase(const FString& SavePath, const FString& FileName, TFunction<void()> Func);

	TMap<FString, FCreatorFBXMetaData> LoadMetaData();
	void SaveMetaData();
	// FBX ����
	void AddMetaData(FString FileName, FString FBXName);
	void RemoveMetaData(FString FileName);
	FCreatorFBXMetaData GetMetaData(FString FileName);

	UFUNCTION()
	void OnCompleteLoadFBX(const FString& FilePath, AActor* ImportedActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FCreatorFBXMetaData> MetaDataMap;

	TQueue<TPair<FString, class URLFProgress*>> LoadQueue;

	// �α��� ����
	FString Token;

	FOnUploadSuccess OnUploadSuccess;
	FOnDownloadSuccess OnDownloadSuccess;

	FString MetaDataFile = "MetaData.json";

	bool bIsLoading = false;
};
