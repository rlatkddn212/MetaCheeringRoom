// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/SW_CreatorStaticObject.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

ASW_CreatorStaticObject::ASW_CreatorStaticObject()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void ASW_CreatorStaticObject::BeginPlay()
{
	Super::BeginPlay();

	// Mat->SetScalarParameterValue(TEXT(""), 0);
}

void ASW_CreatorStaticObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASW_CreatorStaticObject::OnSelected(bool isSelected)
{
	
	if (isSelected)
	{
		Mesh->SetRenderCustomDepth(true);
		UE_LOG(LogTemp, Warning, TEXT("Selected"));
		
	}
	else
	{
		Mesh->SetRenderCustomDepth(false);
		UE_LOG(LogTemp, Warning, TEXT("Unselected"));
	}
}
