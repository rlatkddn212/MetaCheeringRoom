// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorGizmo/CreatorScaleGizmoComponent.h"
#include "../SW_CreatorObject.h"

// Sets default values for this component's properties
UCreatorScaleGizmoComponent::UCreatorScaleGizmoComponent()
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
void UCreatorScaleGizmoComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
    Me = Cast<ASW_CreatorObject>(GetOwner());

    // ...
    if (RedMat)
    {
        Me->XScaleAxisMesh->SetMaterial(0, RedMat);
        Me->XScaleAxisMesh->SetMaterial(1, RedMat);

		Me->XScaleRectMesh->SetMaterial(0, RedMat);
		Me->XScaleRectMesh->SetMaterial(1, RedMat);
    }

    if (GreenMat)
    {
        Me->YScaleAxisMesh->SetMaterial(0, GreenMat);
        Me->YScaleAxisMesh->SetMaterial(1, GreenMat);

		Me->YScaleRectMesh->SetMaterial(0, GreenMat);
		Me->YScaleRectMesh->SetMaterial(1, GreenMat);
    }

    if (BlueMat)
    {
        Me->ZScaleAxisMesh->SetMaterial(0, BlueMat);
        Me->ZScaleAxisMesh->SetMaterial(1, BlueMat);

		Me->ZScaleRectMesh->SetMaterial(0, BlueMat);
		Me->ZScaleRectMesh->SetMaterial(1, BlueMat);
    }
}


// Called every frame
void UCreatorScaleGizmoComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCreatorScaleGizmoComponent::SetGizmoVisibility(bool isVisible)
{
	Me->XScaleAxisMesh->SetVisibility(isVisible);
	Me->YScaleAxisMesh->SetVisibility(isVisible);
	Me->ZScaleAxisMesh->SetVisibility(isVisible);

	Me->XScaleRectMesh->SetVisibility(isVisible);
	Me->YScaleRectMesh->SetVisibility(isVisible);
	Me->ZScaleRectMesh->SetVisibility(isVisible);
}

void UCreatorScaleGizmoComponent::SetAxisSelected(bool isX, bool isY, bool isZ)
{
	// 축 선택
	IsXAxisSelected = isX;
	IsYAxisSelected = isY;
	IsZAxisSelected = isZ;

	// 선택한 축을 노란색으로
	if (isX)
	{
		Me->XScaleAxisMesh->SetMaterial(0, YellowMat);
		Me->XScaleAxisMesh->SetMaterial(1, YellowMat);

		Me->XScaleRectMesh->SetMaterial(0, YellowMat);
		Me->XScaleRectMesh->SetMaterial(1, YellowMat);
	}
	else
	{
		Me->XScaleAxisMesh->SetMaterial(0, RedMat);
		Me->XScaleAxisMesh->SetMaterial(1, RedMat);

		Me->XScaleRectMesh->SetMaterial(0, RedMat);
		Me->XScaleRectMesh->SetMaterial(1, RedMat);
	}

	if (isY)
	{
		Me->YScaleAxisMesh->SetMaterial(0, YellowMat);
		Me->YScaleAxisMesh->SetMaterial(1, YellowMat);

		Me->YScaleRectMesh->SetMaterial(0, YellowMat);
		Me->YScaleRectMesh->SetMaterial(1, YellowMat);
	}
	else
	{
		Me->YScaleAxisMesh->SetMaterial(0, GreenMat);
		Me->YScaleAxisMesh->SetMaterial(1, GreenMat);

		Me->YScaleRectMesh->SetMaterial(0, GreenMat);
		Me->YScaleRectMesh->SetMaterial(1, GreenMat);
	}

	if (isZ)
	{
		Me->ZScaleAxisMesh->SetMaterial(0, YellowMat);
		Me->ZScaleAxisMesh->SetMaterial(1, YellowMat);

		Me->ZScaleRectMesh->SetMaterial(0, YellowMat);
		Me->ZScaleRectMesh->SetMaterial(1, YellowMat);
	}
	else
	{
		Me->ZScaleAxisMesh->SetMaterial(0, BlueMat);
		Me->ZScaleAxisMesh->SetMaterial(1, BlueMat);

		Me->ZScaleRectMesh->SetMaterial(0, BlueMat);
		Me->ZScaleRectMesh->SetMaterial(1, BlueMat);
	}
}