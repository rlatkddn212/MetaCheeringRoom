// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorFBXSubsystem.h"
#include "FileIOBlueprintFunctionLibrary.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "../Plugins/RuntimeLoadStaticMesh/Source/RuntimeLoadFbx/Public/RLFProgress.h"

void UCreatorFBXSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	AnonymousLogin();
	bIsLoading = false;
}

void UCreatorFBXSubsystem::Deinitialize()
{
}

AActor* UCreatorFBXSubsystem::OpenAndCopyFBX(const FString& FilePath, const FString& NewFileName, class URLFProgress* ProgressTracker)
{
	FString CopyPath = FPaths::ProjectSavedDir() + TEXT("FBX/") + NewFileName;

	// FBX 폴더가 없다면 생성한다
	if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*(FPaths::ProjectSavedDir() + TEXT("FBX/")))) {
		FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*(FPaths::ProjectSavedDir() + TEXT("FBX/")));
	}

	FileUploadToFirebase(FilePath, NewFileName);
	if (FPlatformFileManager::Get().GetPlatformFile().CopyFile(*CopyPath, *FilePath)) 
	{
		UE_LOG(LogTemp, Log, TEXT("File moved successfully!"));
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to move file. %s"), *CopyPath);
	}

	AddMetaData(NewFileName, FPaths::GetBaseFilename(FilePath));

	ProgressTracker->finishCal.AddDynamic(this, &UCreatorFBXSubsystem::OnCompleteLoadFBX);
	if (!bIsLoading)
	{
		bIsLoading = true;
		AActor* actor = UFileIOBlueprintFunctionLibrary::LoadFileAsync2StaticMeshActor(FilePath, ProgressTracker);
		return actor;
	}
	else
	{
		LoadQueue.Enqueue(TPair<FString, URLFProgress*>(FilePath, ProgressTracker));
	}
	
	return nullptr;
}

AActor* UCreatorFBXSubsystem::LoadFBX(FString FileName, URLFProgress* Progress)
{
	FString FilePath = FPaths::ProjectSavedDir() + TEXT("FBX/") + FileName;
	Progress->finishCal.AddDynamic(this, &UCreatorFBXSubsystem::OnCompleteLoadFBX);
	if (!bIsLoading)
	{
		bIsLoading = true;
		AActor* actor = UFileIOBlueprintFunctionLibrary::LoadFileAsync2StaticMeshActor(FilePath, Progress);
		return actor;
	}
	else
	{
		LoadQueue.Enqueue(TPair<FString, URLFProgress*>(FilePath, Progress));
	}

	return nullptr;
}

bool UCreatorFBXSubsystem::IsFileExist(const FString& FileName)
{
	FString FilePath = FPaths::ProjectSavedDir() + TEXT("FBX/") + FileName;

	// 파일이 있는지 확인
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("File not found: %s"), *FilePath);
		return false;
	}

	return true;
}

void UCreatorFBXSubsystem::AnonymousLogin()
{
	FString FirebaseAPIKey = "AIzaSyBkWoTtrKuzuH_9XPeUgtfsdLn25CG_ks4";
	FString URL = "https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=" + FirebaseAPIKey;

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(URL);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");

	FString RequestBody = "{\"returnSecureToken\":true}";
	Request->SetContentAsString(RequestBody);

	Request->OnProcessRequestComplete().BindUObject(this, &UCreatorFBXSubsystem::OnAnonymousLoginComplete);
	Request->ProcessRequest();
}

void UCreatorFBXSubsystem::OnAnonymousLoginComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		FString ResponseBody = Response->GetContentAsString();
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseBody);
		if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
		{
			Token = JsonObject->GetStringField("idToken");
			UE_LOG(LogTemp, Log, TEXT("Token: %s"), *Token);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to login"));
	}
}

void UCreatorFBXSubsystem::FileUploadToFirebase(const FString& FilePath, const FString& FileName)
{
	// fbx 3d model upload
	FString URL = "https://firebasestorage.googleapis.com/v0/b/fir-test-4a1cc.appspot.com/o/" + FileName + "?alt=media";
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(URL);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/octet-stream");
	Request->SetHeader("Authorization", "Bearer " + Token);

	TArray<uint8> FileData;
	FFileHelper::LoadFileToArray(FileData, *FilePath);
	Request->SetContent(FileData);

	Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (bWasSuccessful)
			{
				UE_LOG(LogTemp, Log, TEXT("File uploaded successfully"));

				// 성공 델리게이트 호출
				OnUploadSuccess.Broadcast();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to upload file"));
			}
		});

	Request->ProcessRequest();
}

void UCreatorFBXSubsystem::FileDownloadFromFirebase(const FString& SavePath, const FString& FileName, TFunction<void()> Func)
{
	FString URL = "https://firebasestorage.googleapis.com/v0/b/fir-test-4a1cc.appspot.com/o/" + FileName + "?alt=media";
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->SetHeader("Authorization", "Bearer " + Token);

	FString FilePath = SavePath + FileName;
	Request->OnProcessRequestComplete().BindLambda([this, FilePath, Func](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (bWasSuccessful)
			{
				// 응답 코드 확인
				if (Response->GetResponseCode() != 200)
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to download file: %d"), Response->GetResponseCode());
					return;
				}

				if (FFileHelper::SaveArrayToFile(Response->GetContent(), *FilePath))
				{
					UE_LOG(LogTemp, Log, TEXT("File downloaded successfully"));

					// 성공 델리게이트 호출
					Func();
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to download file"));
			}
		});

	Request->ProcessRequest();
}

TMap<FString, FCreatorFBXMetaData> UCreatorFBXSubsystem::LoadMetaData()
{	
	MetaDataMap.Empty();
	FString JsonStr;

	FString MetaDataFilePath = FPaths::ProjectSavedDir() + TEXT("/FBX/") + MetaDataFile;
	if (!FFileHelper::LoadFileToString(JsonStr, *MetaDataFilePath))
	{
		UE_LOG(LogTemp, Log, TEXT("Failed MetaData loaded from %s"), *MetaDataFilePath);
		return MetaDataMap;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonStr);

	FJsonSerializer::Deserialize(JsonReader, JsonObject);
	if (JsonObject == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to deserialize MetaData"));
		return MetaDataMap;
	}

	// MetaData 를 채운다.
	for (auto& Elem : JsonObject->Values)
	{
		FCreatorFBXMetaData NewMetaData;

		TSharedPtr<FJsonObject> MetaDataObject = Elem.Value->AsObject();

		NewMetaData.FileName = MetaDataObject->GetStringField("FileName");
		NewMetaData.FBXName = MetaDataObject->GetStringField("FBXName");

		this->MetaDataMap.Add(NewMetaData.FileName, NewMetaData);
	}

	return MetaDataMap;
}

void UCreatorFBXSubsystem::SaveMetaData()
{
	// MetaData를 저장한다.
	FString JsonStr;

	TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	for (auto& Elem : MetaDataMap)
	{
		TSharedPtr<FJsonObject> MetaDataObject = MakeShareable(new FJsonObject);
		MetaDataObject->SetStringField("FileName", Elem.Value.FileName);
		MetaDataObject->SetStringField("FBXName", Elem.Value.FBXName);

		JsonObject->SetObjectField(Elem.Key, MetaDataObject);
	}

	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&JsonStr);
	FJsonSerializer::Serialize(JsonObject, JsonWriter);

	FString MetaDataFilePath = FPaths::ProjectSavedDir() + TEXT("/FBX/") + MetaDataFile;
	if (FFileHelper::SaveStringToFile(JsonStr, *MetaDataFilePath))
	{
		UE_LOG(LogTemp, Log, TEXT("MetaData saved to %s"), *MetaDataFilePath);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save MetaData to %s"), *MetaDataFilePath);
	}
}

void UCreatorFBXSubsystem::AddMetaData(FString FileName, FString FBXName)
{
	FCreatorFBXMetaData MetaData;
	MetaData.FileName = FileName;
	MetaData.FBXName = FBXName;

	MetaDataMap.Add(FileName, MetaData);
	SaveMetaData();
}

void UCreatorFBXSubsystem::RemoveMetaData(FString FileName)
{
	MetaDataMap.Remove(FileName);
	SaveMetaData();
}

FCreatorFBXMetaData UCreatorFBXSubsystem::GetMetaData(FString FileName)
{
	if (!MetaDataMap.Contains(FileName))
	{
		UE_LOG(LogTemp, Error, TEXT("MetaData not found: %s"), *FileName);
		return FCreatorFBXMetaData();
	}

	return MetaDataMap[FileName];
}

void UCreatorFBXSubsystem::OnCompleteLoadFBX(const FString& FilePath, AActor* ImportedActor)
{
	UE_LOG(LogTemp, Log, TEXT("OnCompleteLoadFBX: %s"), *FilePath);
	// 큐가 남아있다면
	if (!LoadQueue.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("Queue is not empty"));
		TPair<FString, URLFProgress*> Pair;
		LoadQueue.Dequeue(Pair);

		UFileIOBlueprintFunctionLibrary::LoadFileAsync2StaticMeshActor(Pair.Key, Pair.Value);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Queue is empty"));
		bIsLoading = false;
	}
}
