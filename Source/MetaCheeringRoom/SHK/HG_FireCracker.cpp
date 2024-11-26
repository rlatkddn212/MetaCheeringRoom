// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_FireCracker.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"

void AHG_FireCracker::BeginPlay()
{
	Super::BeginPlay();
	this->ItemData.ItemName = "FireCracker";
	InitItemData();

	SetLifeSpan(5.0f);
}

void AHG_FireCracker::Use()
{
	ServerRPCUse();
}


void AHG_FireCracker::ServerRPCUse_Implementation()
{
	MulticastRPCUse(ItemOwner);
}

void AHG_FireCracker::MulticastRPCUse_Implementation(APawn* pawn)
{
	FVector SpawnLocation = pawn->GetActorLocation() + pawn->GetActorUpVector() * 200.0f;
	FRotator SpawnRotation = pawn->GetActorRotation();

	Destroy();
}
