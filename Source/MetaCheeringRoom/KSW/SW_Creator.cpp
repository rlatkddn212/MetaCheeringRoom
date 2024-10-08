// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/SW_Creator.h"

// Sets default values
ASW_Creator::ASW_Creator()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASW_Creator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASW_Creator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASW_Creator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

