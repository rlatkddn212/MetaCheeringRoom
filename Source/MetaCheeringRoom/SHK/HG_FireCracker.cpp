// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_FireCracker.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

void AHG_FireCracker::BeginPlay()
{
	Super::BeginPlay();
	this->ItemData.ItemName = "FireCracker";
	InitItemData();
}

void AHG_FireCracker::Use()
{
	if (Owner)
	{
		FVector SpawnLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 200.0f;
		FRotator SpawnRotation = Owner->GetActorRotation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireCrackerEffect, SpawnLocation, SpawnRotation,true);
		Destroy();
	}
}
