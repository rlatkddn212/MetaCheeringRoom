// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorFBXSubsystem.h"
#include "FileIOBlueprintFunctionLibrary.h"

void UCreatorFBXSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
}

void UCreatorFBXSubsystem::Deinitialize()
{
}

AActor* UCreatorFBXSubsystem::OpenFBX(FString FilePath)
{
	AActor* actor = UFileIOBlueprintFunctionLibrary::LoadFileAsync2StaticMeshActor(FilePath);

	FString FileName = FPaths::GetBaseFilename(FilePath);
	FString CopyPath = FPaths::ProjectSavedDir() + TEXT("/FBX/") + FileName;

	return actor;
}
