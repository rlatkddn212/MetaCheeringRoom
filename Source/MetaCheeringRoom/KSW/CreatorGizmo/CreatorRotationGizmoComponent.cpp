// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorGizmo/CreatorRotationGizmoComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../SW_CreatorObject.h"

// Sets default values for this component's properties
UCreatorRotationGizmoComponent::UCreatorRotationGizmoComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// mat
	RedMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Ksw/Mat/GizmoSharingRed.GizmoSharingRed'"));

	GreenMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Ksw/Mat/GizmoSharingGreen.GizmoSharingGreen'"));

	BlueMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Ksw/Mat/GizmoSharingBlue.GizmoSharingBlue'"));

	YellowMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Ksw/Mat/GizmoSharingYellow.GizmoSharingYellow'"));
}


// Called when the game starts
void UCreatorRotationGizmoComponent::BeginPlay()
{
	Super::BeginPlay();

    // Actor를 가져옴
    Me = Cast<ASW_CreatorObject>(GetOwner());

    // ...
    if (RedMat)
    {
        Me->XRingMesh->SetMaterial(0, RedMat);
    }

    if (GreenMat)
    {
        Me->YRingMesh->SetMaterial(0, GreenMat);
    }

    if (BlueMat)
    {
        Me->ZRingMesh->SetMaterial(0, BlueMat);
    }
}

void UCreatorRotationGizmoComponent::SetGizmoVisibility(bool isVisible)
{
	Me->XRingMesh->SetVisibility(isVisible);
	Me->YRingMesh->SetVisibility(isVisible);
	Me->ZRingMesh->SetVisibility(isVisible);
}

void UCreatorRotationGizmoComponent::SetAxisSelected(bool isX, bool isY, bool isZ)
{
	// 축 선택
	IsXAxisSelected = isX;
	IsYAxisSelected = isY;
	IsZAxisSelected = isZ;

    // 선택한 축을 노란색으로
	if (isX)
	{
		Me->XRingMesh->SetMaterial(0, YellowMat);
	}
	else
	{
		Me->XRingMesh->SetMaterial(0, RedMat);
	}

	if (isY)
	{
		Me->YRingMesh->SetMaterial(0, YellowMat);
	}
	else
	{
		Me->YRingMesh->SetMaterial(0, GreenMat);
	}

	if (isZ)
	{
		Me->ZRingMesh->SetMaterial(0, YellowMat);
	}
	else
	{
		Me->ZRingMesh->SetMaterial(0, BlueMat);
	}
}

void UCreatorRotationGizmoComponent::Drag(FVector2D MouseDownPosition, FVector2D MousePosition)
{

}

