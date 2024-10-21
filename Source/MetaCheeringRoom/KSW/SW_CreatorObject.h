// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SW_CreatorPlayerController.h"
#include "SW_CreatorObject.generated.h"

struct FCreatorObjectData;

UCLASS()
class METACHEERINGROOM_API ASW_CreatorObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASW_CreatorObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnSelected(bool isSelected);

	void DoDestroy();

	// 변환
	void ChangeToolMode(ECreatorToolState state);

	struct FCreatorObjectData* CreatingObjectData;

	UPROPERTY()
	class USceneComponent* Root;
	
	UPROPERTY()
	class UMaterialInstanceDynamic* Mat;
	
	UPROPERTY()
	class UCreatorPositionGizmoComponent* PositionGizmo;
	UPROPERTY()
	class UCreatorRotationGizmoComponent* RotationGizmo;
	UPROPERTY()
	class UCreatorScaleGizmoComponent* ScaleGizmo;

	UPROPERTY()
    UStaticMeshComponent* XAxisMesh;
	
	UPROPERTY()
    UStaticMeshComponent* YAxisMesh;
	
	UPROPERTY()
    UStaticMeshComponent* ZAxisMesh;

	
    /** X, Y, Z 회전용 링 메쉬 */
    UPROPERTY()
    UStaticMeshComponent* XRingMesh;

    UPROPERTY()
    UStaticMeshComponent* YRingMesh;

    UPROPERTY()
    UStaticMeshComponent* ZRingMesh;

	UPROPERTY(EditAnywhere)
    UStaticMeshComponent* XScaleAxisMesh;
	
	UPROPERTY(EditAnywhere)
    UStaticMeshComponent* YScaleAxisMesh;
	
	UPROPERTY(EditAnywhere)
    UStaticMeshComponent* ZScaleAxisMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* XScaleRectMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* YScaleRectMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ZScaleRectMesh;

	UStaticMesh* CreateCylinderMesh(float Radius, float Height, int32 SegmentCount);

	void SelectAxis(bool isX, bool isY, bool isZ);
	void SelectRotationAxis(bool isX, bool isY, bool isZ);
	void SelectScaleAxis(bool isX, bool isY, bool isZ);

	void Drag(FVector2D MouseDownPosition, FVector2D MousePosition);

	void DragEnd(ECreatorToolState ToolState);

	
};
