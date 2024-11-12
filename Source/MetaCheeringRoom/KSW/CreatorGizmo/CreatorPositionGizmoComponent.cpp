// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorGizmo/CreatorPositionGizmoComponent.h"
#include "Components/SceneComponent.h"
#include "../CreatorObject/SW_CreatorObject.h"

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

}

void UCreatorPositionGizmoComponent::OnComponentCreated()
{
	Super::OnComponentCreated();
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

void UCreatorPositionGizmoComponent::SetAxisSelected(bool isX, bool isY, bool isZ)
{
	IsXAxisSelected = isX;
	IsYAxisSelected = isY;
	IsZAxisSelected = isZ;
	
    // ������ ���� ��������� �ٲ۴�.
	if (isX)
	{
		Me->XAxisMesh->SetMaterial(0, YellowMat);
		Me->XAxisMesh->SetMaterial(1, YellowMat);
		GizmoStartLocation = Me->GetActorLocation();
	}
	else
	{
		Me->XAxisMesh->SetMaterial(0, RedMat);
		Me->XAxisMesh->SetMaterial(1, RedMat);
	}

	if (isY)
	{
		Me->YAxisMesh->SetMaterial(0, YellowMat);
		Me->YAxisMesh->SetMaterial(1, YellowMat);
		GizmoStartLocation = Me->GetActorLocation();
	}
	else
	{
		Me->YAxisMesh->SetMaterial(0, GreenMat);
		Me->YAxisMesh->SetMaterial(1, GreenMat);
	}

	if (isZ)
	{
		Me->ZAxisMesh->SetMaterial(0, YellowMat);
		Me->ZAxisMesh->SetMaterial(1, YellowMat);
		GizmoStartLocation = Me->GetActorLocation();
	}
	else
	{
		Me->ZAxisMesh->SetMaterial(0, BlueMat);
		Me->ZAxisMesh->SetMaterial(1, BlueMat);
	}
}

void UCreatorPositionGizmoComponent::Drag(FVector2D MouseDownPosition, FVector2D MousePosition)
{
	ASW_CreatorPlayerController* PC = Cast<ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());
	FVector CurrentPosition = OnMouseClick(MouseDownPosition);
	FVector ClosestPoint = OnMouseClick(MousePosition);

	FVector Delta = ClosestPoint - CurrentPosition;
	FVector NewLocation = GizmoStartLocation + Delta;

	// NewLocation�� �ݿø��Ͽ� ���
	NewLocation.X = FMath::RoundToFloat(NewLocation.X / 10.0f) * 10.0f;
	NewLocation.Y = FMath::RoundToFloat(NewLocation.Y / 10.0f) * 10.0f;
	NewLocation.Z = FMath::RoundToFloat(NewLocation.Z / 10.0f) * 10.0f;

	Me->SetActorLocation(NewLocation);
	PC->OnObjectChanged();
}

FVector UCreatorPositionGizmoComponent::OnMouseClick(const FVector2D& ScreenPosition)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return FVector::ZeroVector;

	// Ŭ�� ��ġ�� �������� Ray ����
	FVector RayOrigin, RayDirection;
	GetClickRay(PlayerController, ScreenPosition, RayOrigin, RayDirection);

	// Actor�� ��ġ�� X�� �������� ���� ����
	FVector LinePoint = Me->XAxisMesh->GetComponentLocation();
	

	if (IsXAxisSelected)
	{
		FVector LineDirection = Me->XAxisMesh->GetForwardVector(); // Actor�� X�� ���� (���� ����)

		// ���� ����� �� ���
		return ClosestPointOnAxisToRay(LinePoint, LineDirection, RayOrigin, RayDirection);
		
	}

	if (IsYAxisSelected)
	{
		FVector LineDirection = Me->YAxisMesh->GetForwardVector(); // Actor�� X�� ���� (���� ����)

		// ���� ����� �� ���
		return ClosestPointOnAxisToRay(LinePoint, LineDirection, RayOrigin, RayDirection);
	}

	if (IsZAxisSelected)
	{
		FVector LineDirection = Me->ZAxisMesh->GetForwardVector(); // Actor�� X�� ���� (���� ����)

		// ���� ����� �� ���
		return ClosestPointOnAxisToRay(LinePoint, LineDirection, RayOrigin, RayDirection);
	}
	
	return FVector::ZeroVector;
	// Debug �ð�ȭ
	//FVector RayEnd = RayOrigin + RayDirection * 1000.0f;

	//DrawDebugLine(GetWorld(), RayOrigin, RayEnd, FColor::Green, false, 5.0f, 0, 2.0f); // Ray
	//DrawDebugLine(GetWorld(), LinePoint, LinePoint + LineDirection * 500.0f, FColor::Red, false, 5.0f, 0, 2.0f); // ����
	//DrawDebugPoint(GetWorld(), ClosestPoint, 10.0f, FColor::Blue, false, 5.0f); // �ֱ��� ��
}

void UCreatorPositionGizmoComponent::GetClickRay(const APlayerController* PlayerController, const FVector2D& ScreenPosition, FVector& RayOrigin, FVector& RayDirection)
{
	FVector WorldPosition, WorldDirection;
	PlayerController->DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldPosition, WorldDirection);

	RayOrigin = WorldPosition;
	RayDirection = WorldDirection.GetSafeNormal();
}

FVector UCreatorPositionGizmoComponent::ClosestPointOnAxisToRay(const FVector& LinePoint, const FVector& LineDirection, const FVector& RayOrigin, const FVector& RayDirection)
{
	// ������ Ray ������ ���� ����� �� ���
	FVector OA = RayOrigin - LinePoint;
	float DDotB = FVector::DotProduct(RayDirection, LineDirection);
	float DDotD = FVector::DotProduct(RayDirection, RayDirection);
	float BDotB = FVector::DotProduct(LineDirection, LineDirection);
	float DDotOA = FVector::DotProduct(RayDirection, OA);
	float BDotOA = FVector::DotProduct(LineDirection, OA);

	float denominator = DDotD * BDotB - DDotB * DDotB;
	if (FMath::IsNearlyZero(denominator)) // �� ���� ������ ��� ó��
	{
		return LinePoint;
	}

	float s = (DDotOA * DDotB - BDotOA * DDotD) / denominator;
	float t = (BDotOA * DDotB - DDotOA * BDotB) / denominator;

	// t�� ������ ��� Ray�� ������ ���
	FVector PointOnLine = LinePoint + s * -LineDirection;
	FVector PointOnRay = RayOrigin + FMath::Max(t, 0.0f) * RayDirection;

	return PointOnLine;
}

