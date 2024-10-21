// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CreatorPositionGizmoComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class METACHEERINGROOM_API UCreatorPositionGizmoComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCreatorPositionGizmoComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
   
	/** ����� Ȱ��ȭ �Լ� */
    UFUNCTION(BlueprintCallable)
	void SetGizmoVisibility(bool isVisible);

	// �� ����
	void SetAxisSelected(bool isX, bool isY, bool isZ);

	void Drag(FVector2D MouseDownPosition, FVector2D MousePosition);
	FVector OnMouseClick(const FVector2D& ScreenPosition);
	void GetClickRay(const APlayerController* PlayerController, const FVector2D& ScreenPosition, FVector& RayOrigin, FVector& RayDirection);

	FVector ClosestPointOnAxisToRay(const FVector& LinePoint, const FVector& LineDirection, const FVector& RayOrigin, const FVector& RayDirection);
	
	// ������ �� ���� ����
	UPROPERTY()
	bool IsXAxisSelected;
	UPROPERTY()
	bool IsYAxisSelected;
	UPROPERTY()
	bool IsZAxisSelected;

	// ���͸��� ����
	UPROPERTY()
	class UMaterialInterface* RedMat;

	UPROPERTY()
	class UMaterialInterface* GreenMat;
	
	UPROPERTY()
	class UMaterialInterface* BlueMat;
	
	UPROPERTY()
	class UMaterialInterface* YellowMat;

	UPROPERTY()
	class ASW_CreatorObject* Me;

	UPROPERTY()
	FVector GizmoStartLocation;
};
