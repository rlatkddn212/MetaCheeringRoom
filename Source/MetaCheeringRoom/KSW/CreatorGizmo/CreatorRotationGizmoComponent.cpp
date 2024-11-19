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
	if (Me)
	{
		// 축 선택
		IsXAxisSelected = isX;
		IsYAxisSelected = isY;
		IsZAxisSelected = isZ;

		// 선택한 축을 노란색으로
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

	// Me의 Screen상의 좌표를 가져옴
	FVector2D ScreenPosition;
	PC->ProjectWorldLocationToScreen(Me->GetActorLocation(), ScreenPosition);
	//ScreenPosition 좌표 기준으로 MouseDownPosition, MousePosition 90도 회전시킴
	FVector2D MouseDownPosition90 = FVector2D(ScreenPosition.X + (ScreenPosition.Y - MouseDownPosition.Y), ScreenPosition.Y - (ScreenPosition.X - MouseDownPosition.X));
	FVector2D MousePosition90 = FVector2D(ScreenPosition.X + (ScreenPosition.Y - MousePosition.Y), ScreenPosition.Y - (ScreenPosition.X - MousePosition.X));


	if (IsXAxisSelected)
	{
		FVector CurrentPosition = OnMouseClick(MouseDownPosition90);
		FVector ClosestPoint = OnMouseClick(MousePosition90);

		FVector Delta = ClosestPoint - CurrentPosition;

		// Delta를 이용하여 회전값 계산
		if (Delta.Size() < 0.1f) return;

		//Me->XRingMesh 의 방향 벡터와 Delta의 방향 벡터를 내적하여 회전값 계산
		FVector rAxis = Me->XRingMesh->GetUpVector();
		FVector DeltaDirection = Delta.GetSafeNormal();

		FQuat RotationDelta;
		double AngleRad = Delta.Size() * 0.01f;
		// 라디안을 디그리고 변경 10단위로 반올림
		double angle = FMath::RadiansToDegrees(AngleRad);
		angle = FMath::RoundToDouble(angle / 10.0f) * 10.0f;
		AngleRad = FMath::DegreesToRadians(angle);
		RotationDelta = FQuat(DeltaDirection, AngleRad);
		
		FQuat NewRotation = RotationDelta * CurrentRotation;

		// 회전값 적용 10단위
		Me->Server_SetRotation(NewRotation);
		PC->OnObjectChanged();
	}

	if (IsYAxisSelected)
	{
		FVector CurrentPosition = OnMouseClick(MouseDownPosition90);
		FVector ClosestPoint = OnMouseClick(MousePosition90);

		FVector Delta = ClosestPoint - CurrentPosition;

		// Delta를 이용하여 회전값 계산
		//Me->XRingMesh 의 방향 벡터와 Delta의 방향 벡터를 내적하여 회전값 계산
		FVector rAxis = Me->YRingMesh->GetUpVector();

		FVector DeltaDirection = Delta.GetSafeNormal();
		
		FQuat RotationDelta;

		double AngleRad = Delta.Size() * 0.01f;
		// 라디안을 디그리고 변경 10단위로 반올림
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
		// Delta를 이용하여 회전값 계산
		FVector DeltaDirection = Delta.GetSafeNormal();

		FQuat RotationDelta;

		double AngleRad = Delta.Size() * 0.01f;
		// 라디안을 디그리고 변경 10단위로 반올림
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

	// 클릭 위치를 기준으로 Ray 생성
	FVector RayOrigin, RayDirection;
	GetClickRay(PlayerController, ScreenPosition, RayOrigin, RayDirection);

	// Actor의 위치와 X축 방향으로 직선 정의
	FVector LinePoint = Me->XRingMesh->GetComponentLocation();


	if (IsXAxisSelected)
	{
		FVector LineDirection = RotationAxis; // Actor의 X축 방향 (로컬 기준)

		// 가장 가까운 점 계산
		return ClosestPointOnAxisToRay(LinePoint, LineDirection, RayOrigin, RayDirection);

	}

	if (IsYAxisSelected)
	{
		FVector LineDirection = RotationAxis; // Actor의 X축 방향 (로컬 기준)

		// 가장 가까운 점 계산
		return ClosestPointOnAxisToRay(LinePoint, LineDirection, RayOrigin, RayDirection);
	}

	if (IsZAxisSelected)
	{
		FVector LineDirection = RotationAxis; // Actor의 X축 방향 (로컬 기준)

		// 가장 가까운 점 계산
		return ClosestPointOnAxisToRay(LinePoint, LineDirection, RayOrigin, RayDirection);
	}

	return FVector::ZeroVector;
	// Debug 시각화
	//FVector RayEnd = RayOrigin + RayDirection * 1000.0f;

	//DrawDebugLine(GetWorld(), RayOrigin, RayEnd, FColor::Green, false, 5.0f, 0, 2.0f); // Ray
	//DrawDebugLine(GetWorld(), LinePoint, LinePoint + LineDirection * 500.0f, FColor::Red, false, 5.0f, 0, 2.0f); // 직선
	//DrawDebugPoint(GetWorld(), ClosestPoint, 10.0f, FColor::Blue, false, 5.0f); // 최근접 점
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
	// 직선과 Ray 사이의 가장 가까운 점 계산
	FVector OA = RayOrigin - LinePoint;
	float DDotB = FVector::DotProduct(RayDirection, LineDirection);
	float DDotD = FVector::DotProduct(RayDirection, RayDirection);
	float BDotB = FVector::DotProduct(LineDirection, LineDirection);
	float DDotOA = FVector::DotProduct(RayDirection, OA);
	float BDotOA = FVector::DotProduct(LineDirection, OA);

	float denominator = DDotD * BDotB - DDotB * DDotB;
	if (FMath::IsNearlyZero(denominator)) // 두 선이 평행한 경우 처리
	{
		return LinePoint;
	}

	float s = (DDotOA * DDotB - BDotOA * DDotD) / denominator;
	float t = (BDotOA * DDotB - DDotOA * BDotB) / denominator;

	// t가 음수일 경우 Ray의 시작점 사용
	FVector PointOnLine = LinePoint + s * -LineDirection;
	FVector PointOnRay = RayOrigin + FMath::Max(t, 0.0f) * RayDirection;

	return PointOnLine;
}

