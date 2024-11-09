// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorFBXSubsystem.h"
#include "FileIOBlueprintFunctionLibrary.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

void UCreatorFBXSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	AnonymousLogin();
}

void UCreatorFBXSubsystem::Deinitialize()
{
}

AActor* UCreatorFBXSubsystem::OpenAndCopyFBX(const FString& FilePath, const FString& NewFileName)
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

	AActor* actor = UFileIOBlueprintFunctionLibrary::LoadFileAsync2StaticMeshActor(FilePath);

	return actor;
}

AActor* UCreatorFBXSubsystem::LoadFBX(FString FileName)
{
	FString FilePath = FPaths::ProjectSavedDir() + TEXT("FBX/") + FileName;

	return UFileIOBlueprintFunctionLibrary::LoadFileAsync2StaticMeshActor(FilePath);
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
