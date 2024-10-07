// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_ItemBase.h"

// Sets default values
AHG_ItemBase::AHG_ItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHG_ItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHG_ItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

