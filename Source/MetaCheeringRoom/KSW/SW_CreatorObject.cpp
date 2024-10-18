// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/SW_CreatorObject.h"
#include "CreatorStorageSubsystem.h"

// Sets default values
ASW_CreatorObject::ASW_CreatorObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASW_CreatorObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASW_CreatorObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASW_CreatorObject::OnSelected(bool isSelected)
{
	if (isSelected)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected"));
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unselected"));
	}
}

void ASW_CreatorObject::DoDestroy()
{
	Destroy();
}

