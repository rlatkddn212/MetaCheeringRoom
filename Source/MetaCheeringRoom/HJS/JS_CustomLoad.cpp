// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_CustomLoad.h"
#include "../KSW/CreatorStorageSubsystem.h"
#include "../KSW/CreatorStorageSubsystem.h"
#include "../KSW/CreatorMapSubsystem.h"

// Sets default values
AJS_CustomLoad::AJS_CustomLoad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJS_CustomLoad::BeginPlay()
{
	Super::BeginPlay();
	UCreatorStorageSubsystem* storage = GetGameInstance()->GetSubsystem<UCreatorStorageSubsystem>();
	TArray<FCreatorMapMetaData*> meta = storage->GetCreatorMapMetaDatas();

	FString path = FPaths::ProjectSavedDir() + TEXT("/CreatorMap/") + meta[0]->FileName;
	FString JsonStr = storage->LoadCreatorMap(path);

	UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
	system->SetMapName(meta[0]->CreatorMapName);
	system->SetupJson(JsonStr);

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC->HasAuthority())
	{
		system->LoadMap();
		SetOwner(PC);
	}
}

// Called every frame
void AJS_CustomLoad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

