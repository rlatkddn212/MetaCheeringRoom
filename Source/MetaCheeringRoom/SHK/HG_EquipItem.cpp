
// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_EquipItem.h"
#include "HG_EquipItem.h"

// Sets default values
AHG_EquipItem::AHG_EquipItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}

// Called when the game starts or when spawned
void AHG_EquipItem::BeginPlay()
{
	Super::BeginPlay();
	this->ItemData.ItemName = Name;
	InitItemData();
}

// Called every frame
void AHG_EquipItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHG_EquipItem::Equiped(APawn* Value)
{
	SetOwner(Value);
}

