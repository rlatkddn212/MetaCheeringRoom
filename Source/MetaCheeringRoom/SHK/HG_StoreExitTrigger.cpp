// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_StoreExitTrigger.h"
#include "HG_Player.h"
#include "Components/BoxComponent.h"

AHG_StoreExitTrigger::AHG_StoreExitTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionComp"));
	SetRootComponent(BoxCollisionComp);
	BoxCollisionComp->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
}

void AHG_StoreExitTrigger::BeginPlay()
{
	BoxCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AHG_StoreExitTrigger::OnMyBeginOverlap);
}

void AHG_StoreExitTrigger::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto* Player = Cast<AHG_Player>(OtherActor))
	{
		Player->ExitTheStore();
	}
}
