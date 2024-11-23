// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_ChairCollision.h"
#include "Components/SphereComponent.h"

// Sets default values
AHG_ChairCollision::AHG_ChairCollision()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);
}

// Called when the game starts or when spawned
void AHG_ChairCollision::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHG_ChairCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

