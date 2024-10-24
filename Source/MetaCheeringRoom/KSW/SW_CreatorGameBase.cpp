// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/SW_CreatorGameBase.h"
#include "CreatorMapSubsystem.h"
#include "CreatorStorageSubsystem.h"

ASW_CreatorGameBase::ASW_CreatorGameBase()
{

}

void ASW_CreatorGameBase::BeginPlay()
{
	Super::BeginPlay();

	UCreatorStorageSubsystem* storage = GetGameInstance()->GetSubsystem<UCreatorStorageSubsystem>();

	FString path = FPaths::ProjectContentDir() + TEXT("CreatorMap.json");
	FString str = storage->LoadCreatorMap(path);

	UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
	system->LoadCreatorMapFromJson(str);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *str);
}
