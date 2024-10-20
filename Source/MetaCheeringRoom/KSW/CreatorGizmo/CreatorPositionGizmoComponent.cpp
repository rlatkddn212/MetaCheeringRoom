// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorGizmo/CreatorPositionGizmoComponent.h"
#include "Components/SceneComponent.h"
#include "../SW_CreatorObject.h"

// Sets default values for this component's properties
UCreatorPositionGizmoComponent::UCreatorPositionGizmoComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    // mat
    RedMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Ksw/Mat/GizmoSharingRed.GizmoSharingRed'"));

    GreenMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Ksw/Mat/GizmoSharingGreen.GizmoSharingGreen'"));


	BlueMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Ksw/Mat/GizmoSharingBlue.GizmoSharingBlue'"));


	YellowMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Ksw/Mat/GizmoSharingYellow.GizmoSharingYellow'"));

}


// Called when the game starts
void UCreatorPositionGizmoComponent::BeginPlay()
{
	Super::BeginPlay();

	Me = Cast<ASW_CreatorObject>(GetOwner());

	// ...
    if (RedMat)
    {
        Me->XAxisMesh->SetMaterial(0, RedMat);
        Me->XAxisMesh->SetMaterial(1, RedMat);
    }

    if (GreenMat)
    {
        Me->YAxisMesh->SetMaterial(0, GreenMat);
        Me->YAxisMesh->SetMaterial(1, GreenMat);
    }

    if (BlueMat)
    {
        Me->ZAxisMesh->SetMaterial(0, BlueMat);
        Me->ZAxisMesh->SetMaterial(1, BlueMat);
    }
}

void UCreatorPositionGizmoComponent::SetGizmoVisibility(bool isVisible)
{
	Me->XAxisMesh->SetVisibility(isVisible);
	Me->YAxisMesh->SetVisibility(isVisible);
	Me->ZAxisMesh->SetVisibility(isVisible);
}

