// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_FireCracker.h"
#include "Kismet/GameplayStatics.h"

void AHG_FireCracker::BeginPlay()
{
	Super::BeginPlay();
	this->ItemData.ItemName = "FireCracker";
	InitItemData();
}

void AHG_FireCracker::Use()
{
	FVector SpawnLocation = FVector::ZeroVector;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),FireCrackerEffect,SpawnLocation,SpawnRotation);
}
