// Fill out your copyright notice in the Description page of Project Settings.


#include "SW_CreatorStaticObject.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "KSW/CreatorGizmo/CreatorPositionGizmoComponent.h"
#include "KSW/CreatorGizmo/CreatorRotationGizmoComponent.h"
#include "KSW/CreatorGizmo/CreatorScaleGizmoComponent.h"

ASW_CreatorStaticObject::ASW_CreatorStaticObject()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void ASW_CreatorStaticObject::BeginPlay()
{
	Super::BeginPlay();
}

void ASW_CreatorStaticObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASW_CreatorStaticObject::OnSelected(bool isSelected)
{
	Super::OnSelected(isSelected);

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
