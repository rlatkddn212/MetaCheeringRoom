// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_StarActor.h"

// Sets default values
AJS_StarActor::AJS_StarActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StarMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("StarMesh"));
	SetRootComponent(StarMesh);
}

// Called when the game starts or when spawned
void AJS_StarActor::BeginPlay()
{
	Super::BeginPlay();

    FTimerHandle DestroyHandle;
    GetWorld()->GetTimerManager().SetTimer(DestroyHandle, [this]()
        {
            // Destroy the actor
            this->Destroy();
        }, 2.f, false);
}

// Called every frame
void AJS_StarActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

