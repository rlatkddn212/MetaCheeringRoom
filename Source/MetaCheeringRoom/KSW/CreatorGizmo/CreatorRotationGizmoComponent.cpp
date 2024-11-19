// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorGizmo/CreatorRotationGizmoComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../CreatorObject/SW_CreatorObject.h"

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

}

void UCreatorRotationGizmoComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	// Actor�� ������
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
	if (Me)
	{
		// �� ����
		IsXAxisSelected = isX;
		IsYAxisSelected = isY;
		IsZAxisSelected = isZ;

		// ������ ���� ���������
		if (isX)
		{
			Me->XRingMesh->SetMaterial(0, YellowMat);
			CurrentRotation = Me->GetActorQuat();
			RotationAxis = Me->XRingMesh->GetUpVector();

		}
		else
		{
			Me->XRingMesh->SetMaterial(0, RedMat);

		}

		if (isY)
		{
			Me->YRingMesh->SetMaterial(0, YellowMat);
			CurrentRotation = Me->GetActorQuat();
			RotationAxis = Me->YRingMesh->GetUpVector();
		}
		else
		{
			Me->YRingMesh->SetMaterial(0, GreenMat);
		}

		if (isZ)
		{
			Me->ZRingMesh->SetMaterial(0, YellowMat);
			CurrentRotation = Me->GetActorQuat();
			RotationAxis = Me->ZRingMesh->GetUpVector();
		}
		else
		{
			Me->ZRingMesh->SetMaterial(0, BlueMat);
		}
	}
}

void UCreatorRotationGizmoComponent::Drag(FVector2D MouseDownPosition, FVector2D MousePosition)
{
	ASW_CreatorPlayerController* PC = Cast<ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());

	// Me�� Screen���� ��ǥ�� ������
	FVector2D ScreenPosition;
	PC->ProjectWorldLocationToScreen(Me->GetActorLocation(), ScreenPosition);
	//ScreenPosition ��ǥ �������� MouseDownPosition, MousePosition 90�� ȸ����Ŵ
	FVector2D MouseDownPosition90 = FVector2D(ScreenPosition.X + (ScreenPosition.Y - MouseDownPosition.Y), ScreenPosition.Y - (ScreenPosition.X - MouseDownPosition.X));
	FVector2D MousePosition90 = FVector2D(ScreenPosition.X + (ScreenPosition.Y - MousePosition.Y), ScreenPosition.Y - (ScreenPosition.X - MousePosition.X));


	if (IsXAxisSelected)
	{
		FVector CurrentPosition = OnMouseClick(MouseDownPosition90);
		FVector ClosestPoint = OnMouseClick(MousePosition90);

		FVector Delta = ClosestPoint - CurrentPosition;

		// Delta�� �̿��Ͽ� ȸ���� ���
		if (Delta.Size() < 0.1f) return;

		//Me->XRingMesh �� ���� ���Ϳ� Delta�� ���� ���͸� �����Ͽ� ȸ���� ���
		FVector rAxis = Me->XRingMesh->GetUpVector();
		FVector DeltaDirection = Delta.GetSafeNormal();

		FQuat RotationDelta;
		double AngleRad = Delta.Size() * 0.01f;
		// ������ ��׸��� ���� 10������ �ݿø�
		double angle = FMath::RadiansToDegrees(AngleRad);
		angle = FMath::RoundToDouble(angle / 10.0f) * 10.0f;
		AngleRad = FMath::DegreesToRadians(angle);
		RotationDelta = FQuat(DeltaDirection, AngleRad);
		
		FQuat NewRotation = RotationDelta * CurrentRotation;

		// ȸ���� ���� 10����
		Me->Server_SetRotation(NewRotation);
		PC->OnObjectChanged();
	}

	if (IsYAxisSelected)
	{
		FVector CurrentPosition = OnMouseClick(MouseDownPosition90);
		FVector ClosestPoint = OnMouseClick(MousePosition90);

		FVector Delta = ClosestPoint - CurrentPosition;

		// Delta�� �̿��Ͽ� ȸ���� ���
		//Me->XRingMesh �� ���� ���Ϳ� Delta�� ���� ���͸� �����Ͽ� ȸ���� ���
		FVector rAxis = Me->YRingMesh->GetUpVector();

		FVector DeltaDirection = Delta.GetSafeNormal();
		
		FQuat RotationDelta;

		double AngleRad = Delta.Size() * 0.01f;
		// ������ ��׸��� ���� 10������ �ݿø�
		double angle = FMath::RadiansToDegrees(AngleRad);
		angle = FMath::RoundToDouble(angle / 10.0f) * 10.0f;
		AngleRad = FMath::DegreesToRadians(angle);

		RotationDelta = FQuat(DeltaDirection, AngleRad);
		
		FQuat NewRotation = RotationDelta * CurrentRotation;
		Me->Server_SetRotation(NewRotation);
		PC->OnObjectChanged();
	}

	if (IsZAxisSelected)
	{
		FVector CurrentPosition = OnMouseClick(MouseDownPosition90);
		FVector ClosestPoint = OnMouseClick(MousePosition90);

		FVector Delta = ClosestPoint - CurrentPosition;
		FVector rAxis = Me->ZRingMesh->GetUpVector();
		// Delta�� �̿��Ͽ� ȸ���� ���
		FVector DeltaDirection = Delta.GetSafeNormal();

		FQuat RotationDelta;

		double AngleRad = Delta.Size() * 0.01f;
		// ������ ��׸��� ���� 10������ �ݿø�
		double angle = FMath::RadiansToDegrees(AngleRad);
		angle = FMath::RoundToDouble(angle / 10.0f) * 10.0f;
		AngleRad = FMath::DegreesToRadians(angle);
		
		RotationDelta = FQuat(DeltaDirection, AngleRad);
		
		FQuat NewRotation = RotationDelta * CurrentRotation;
		Me->Server_SetRotation(NewRotation);
		PC->OnObjectChanged();
	}
}


FVector UCreatorRotationGizmoComponent::OnMouseClick(const FVector2D& ScreenPosition)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return FVector::ZeroVector;

	// Ŭ�� ��ġ�� �������� Ray ����
	FVector RayOrigin, RayDirection;
	GetClickRay(PlayerController, ScreenPosition, RayOrigin, RayDirection);

	// Actor�� ��ġ�� X�� �������� ���� ����
	FVector LinePoint = Me->XRingMesh->GetComponentLocation();


	if (IsXAxisSelected)
	{
		FVector LineDirection = RotationAxis; // Actor�� X�� ���� (���� ����)

		// ���� ����� �� ���
		return ClosestPointOnAxisToRay(LinePoint, LineDirection, RayOrigin, RayDirection);

	}

	if (IsYAxisSelected)
	{
		FVector LineDirection = RotationAxis; // Actor�� X�� ���� (���� ����)

		// ���� ����� �� ���
		return ClosestPointOnAxisToRay(LinePoint, LineDirection, RayOrigin, RayDirection);
	}

	if (IsZAxisSelected)
	{
		FVector LineDirection = RotationAxis; // Actor�� X�� ���� (���� ����)

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

void UCreatorRotationGizmoComponent::GetClickRay(const APlayerController* PlayerController, const FVector2D& ScreenPosition, FVector& RayOrigin, FVector& RayDirection)
{
	FVector WorldPosition, WorldDirection;
	PlayerController->DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldPosition, WorldDirection);

	RayOrigin = WorldPosition;
	RayDirection = WorldDirection.GetSafeNormal();
}

FVector UCreatorRotationGizmoComponent::ClosestPointOnAxisToRay(const FVector& LinePoint, const FVector& LineDirection, const FVector& RayOrigin, const FVector& RayDirection)
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

