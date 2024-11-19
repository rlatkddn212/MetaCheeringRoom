// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorGizmo/CreatorScaleGizmoComponent.h"
#include "../CreatorObject/SW_CreatorObject.h"

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

}

void UCreatorScaleGizmoComponent::OnComponentCreated()
{
	Super::OnComponentCreated();
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
	// �� ����
	IsXAxisSelected = isX;
	IsYAxisSelected = isY;
	IsZAxisSelected = isZ;

	// ������ ���� ���������
	if (isX)
	{
		Me->XScaleAxisMesh->SetMaterial(0, YellowMat);
		Me->XScaleAxisMesh->SetMaterial(1, YellowMat);

		Me->XScaleRectMesh->SetMaterial(0, YellowMat);
		Me->XScaleRectMesh->SetMaterial(1, YellowMat);
		GizmoStartScale = Me->GetActorScale();
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
		GizmoStartScale = Me->GetActorScale();
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
		GizmoStartScale = Me->GetActorScale();
	}
	else
	{
		Me->ZScaleAxisMesh->SetMaterial(0, BlueMat);
		Me->ZScaleAxisMesh->SetMaterial(1, BlueMat);

		Me->ZScaleRectMesh->SetMaterial(0, BlueMat);
		Me->ZScaleRectMesh->SetMaterial(1, BlueMat);
	}
}


void UCreatorScaleGizmoComponent::Drag(FVector2D MouseDownPosition, FVector2D MousePosition)
{
	ASW_CreatorPlayerController* PC = Cast<ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());

	if (IsXAxisSelected)
	{
		FVector CurrentPosition = OnMouseClick(MouseDownPosition);
		FVector ClosestPoint = OnMouseClick(MousePosition);

		FVector Delta = (ClosestPoint - CurrentPosition) * Factor;
		FVector LineDirection = Me->XScaleAxisMesh->GetForwardVector();
		float fDir = 1;
		// Delta �� LineDirection�� ���� �������� ���������� �˻�
		if (FVector::DotProduct(Delta, LineDirection) < 0)
		{
			fDir *= -1;
		}

		FVector X = FVector(fDir, 0, 0) * Delta.Size();
		FVector scaleVector = GizmoStartScale + X;

		scaleVector.X = FMath::RoundToFloat(scaleVector.X * 10.0f) / 10.0f;
		scaleVector.Y = FMath::RoundToFloat(scaleVector.Y * 10.0f) / 10.0f;
		scaleVector.Z = FMath::RoundToFloat(scaleVector.Z * 10.0f) / 10.0f;

		Me->Server_SetScale(scaleVector);
		PC->OnObjectChanged();
		// Delta Debugline
		//DrawDebugLine(GetWorld(), CurrentPosition, CurrentPosition + Delta * 1000.0f, FColor::Red, false, 5.0f, 0, 2.0f);
	}

	if (IsYAxisSelected)
	{
		FVector CurrentPosition = OnMouseClick(MouseDownPosition);
		FVector ClosestPoint = OnMouseClick(MousePosition);

		FVector Delta = (ClosestPoint - CurrentPosition) * Factor;
		FVector LineDirection = Me->YScaleAxisMesh->GetForwardVector();
		float fDir = 1;
		// Delta �� LineDirection�� ���� �������� ���������� �˻�
		if (FVector::DotProduct(Delta, LineDirection) < 0)
		{
			fDir *= -1;
		}

		FVector Y = FVector(0, fDir, 0) * Delta.Size();

		FVector scaleVector = GizmoStartScale + Y;

		scaleVector.X = FMath::RoundToFloat(scaleVector.X * 10.0f) / 10.0f;
		scaleVector.Y = FMath::RoundToFloat(scaleVector.Y * 10.0f) / 10.0f;
		scaleVector.Z = FMath::RoundToFloat(scaleVector.Z * 10.0f) / 10.0f;

		Me->Server_SetScale(scaleVector);
		PC->OnObjectChanged();
		//DrawDebugLine(GetWorld(), CurrentPosition, CurrentPosition + Delta * 1000.0f, FColor::Red, false, 5.0f, 0, 2.0f);
	}

	if (IsZAxisSelected)
	{
		FVector CurrentPosition = OnMouseClick(MouseDownPosition);
		FVector ClosestPoint = OnMouseClick(MousePosition);

		FVector Delta = (ClosestPoint - CurrentPosition) * Factor;
		FVector LineDirection = Me->ZScaleAxisMesh->GetForwardVector();
		float fDir = 1;
		// Delta �� LineDirection�� ���� �������� ���������� �˻�
		if (FVector::DotProduct(Delta, LineDirection) < 0)
		{
			fDir *= -1;
		}
		// z �� �������� Ű���.
		// Me->SetActorScale3D(GizmoStartScale + Delta);
		FVector Z = FVector(0, 0, fDir) * Delta.Size();

		FVector scaleVector = GizmoStartScale + Z;

		scaleVector.X = FMath::RoundToFloat(scaleVector.X * 10.0f) / 10.0f;
		scaleVector.Y = FMath::RoundToFloat(scaleVector.Y * 10.0f) / 10.0f;
		scaleVector.Z = FMath::RoundToFloat(scaleVector.Z * 10.0f) / 10.0f;

		Me->Server_SetScale(scaleVector);
		PC->OnObjectChanged();
		//DrawDebugLine(GetWorld(), CurrentPosition, CurrentPosition + Delta * 1000.0f, FColor::Red, false, 5.0f, 0, 2.0f);
	}
}

FVector UCreatorScaleGizmoComponent::OnMouseClick(const FVector2D& ScreenPosition)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return FVector::ZeroVector;

	// Ŭ�� ��ġ�� �������� Ray ����
	FVector RayOrigin, RayDirection;
	GetClickRay(PlayerController, ScreenPosition, RayOrigin, RayDirection);

	// Actor�� ��ġ�� X�� �������� ���� ����
	FVector LinePoint = Me->XScaleAxisMesh->GetComponentLocation();
	if (IsXAxisSelected)
	{
		FVector LineDirection = Me->XScaleAxisMesh->GetForwardVector(); // Actor�� X�� ���� (���� ����)

		// ���� ����� �� ���
		return ClosestPointOnAxisToRay(LinePoint, LineDirection, RayOrigin, RayDirection);

	}

	if (IsYAxisSelected)
	{
		FVector LineDirection = Me->YScaleAxisMesh->GetForwardVector(); // Actor�� X�� ���� (���� ����)

		// ���� ����� �� ���
		return ClosestPointOnAxisToRay(LinePoint, LineDirection, RayOrigin, RayDirection);
	}

	if (IsZAxisSelected)
	{
		FVector LineDirection = Me->ZScaleAxisMesh->GetForwardVector(); // Actor�� X�� ���� (���� ����)

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

void UCreatorScaleGizmoComponent::GetClickRay(const APlayerController* PlayerController, const FVector2D& ScreenPosition, FVector& RayOrigin, FVector& RayDirection)
{
	FVector WorldPosition, WorldDirection;
	PlayerController->DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldPosition, WorldDirection);

	RayOrigin = WorldPosition;
	RayDirection = WorldDirection.GetSafeNormal();
}

FVector UCreatorScaleGizmoComponent::ClosestPointOnAxisToRay(const FVector& LinePoint, const FVector& LineDirection, const FVector& RayOrigin, const FVector& RayDirection)
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

