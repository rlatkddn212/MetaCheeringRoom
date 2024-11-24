// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/Creator/CreatorObjectRootActor.h"
#include "../CreatorMapSubsystem.h"
#include "../SW_CreatorPlayerController.h"

// Sets default values
ACreatorObjectRootActor::ACreatorObjectRootActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

}

// Called when the game starts or when spawned
void ACreatorObjectRootActor::BeginPlay()
{
	Super::BeginPlay();

	// PC¸¦ °¡Á®¿È
	if (!HasAuthority())
	{
		UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
		system->SetRootActor(this);
	}
}

// Called every frame
void ACreatorObjectRootActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

