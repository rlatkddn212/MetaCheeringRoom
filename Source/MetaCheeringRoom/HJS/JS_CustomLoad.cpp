// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_CustomLoad.h"
#include "../KSW/CreatorStorageSubsystem.h"
#include "../KSW/CreatorStorageSubsystem.h"
#include "../KSW/CreatorMapSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "JS_Screen.h"

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

	GetWorldTimerManager().SetTimer(A, this, &AJS_CustomLoad::AA, 20.f, false);
	GetWorldTimerManager().SetTimer(B, this, &AJS_CustomLoad::BB, 30.f, false);
	GetWorldTimerManager().SetTimer(C, this, &AJS_CustomLoad::CC, 65.f, false);

}

// Called every frame
void AJS_CustomLoad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJS_CustomLoad::AA()
{
	// 불끄기
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(),TEXT("EmissiveControlLight"),Actors);
	for (AActor* Act : Actors)
	{
		Act->SetActorHiddenInGame(true);
	}
}

void AJS_CustomLoad::BB()
{
	AJS_Screen* Screen = Cast<AJS_Screen>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_Screen::StaticClass()));

	if (Screen)
	{
		Screen->PlayVOD(TEXT("http://221.163.19.142:13333/vod/시연.mp4"));
	}
}

