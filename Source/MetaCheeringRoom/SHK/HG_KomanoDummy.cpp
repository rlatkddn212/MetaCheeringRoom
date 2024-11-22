// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_KomanoDummy.h"

// Sets default values
AHG_KomanoDummy::AHG_KomanoDummy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHG_KomanoDummy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHG_KomanoDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHG_KomanoDummy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

