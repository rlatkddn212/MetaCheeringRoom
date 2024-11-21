// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KSW/SW_CreatorPlayerController.h"
#include "../CreatorMapSubsystem.h"
#include "SW_CreatorObject.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FChangeSelected, bool, isSelected);

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

	void RecursiveDestroy(AActor* Actor);
	virtual void Destroyed() override;

	virtual void OnSelected(bool isSelected);

	virtual void OnChangeProperty(int32 id, UCreatorPropertyBase* CreatorProperty);
	
	virtual TMap<int32, UCreatorPropertyBase*> GetPropertyMap() { return PropertyMap; }

	void DoDestroy();

	// 기즈모 사이즈 변경
	void SetGizmoSize(float size);

	// 변환
	void ChangeToolMode(ECreatorToolState state);

	UFUNCTION(Server, Reliable)
	void Server_SetLocation(FVector Pos);
	UFUNCTION(Server, Reliable)
	void Server_SetRotation(FQuat Rot);
	UFUNCTION(Server, Reliable)
	void Server_SetScale(FVector Scale);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetScale(FVector Scale);

	struct FCreatorObjectData* CreatingObjectData;

	int32 CreatorObjectType;
	int32 CreatorObjectId;
	int32 CreatorObjectUId;

	FChangeSelected OnChangeSelected;

	UPROPERTY()
	class USceneComponent* Root;
	
	UPROPERTY()
	class UMaterialInstanceDynamic* Mat;
	
    UPROPERTY(EditAnywhere)
	class USceneComponent* GizmoRoot;

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
	
	UPROPERTY()
    UStaticMeshComponent* XYAxisMeshX;
	UPROPERTY()
    UStaticMeshComponent* XYAxisMeshY;

	UPROPERTY()
    UStaticMeshComponent* XZAxisMeshX;
	UPROPERTY()
    UStaticMeshComponent* XZAxisMeshZ;

	UPROPERTY()
    UStaticMeshComponent* YZAxisMeshY;
	UPROPERTY()
    UStaticMeshComponent* YZAxisMeshZ;

	
    /** X, Y, Z 회전용 링 메쉬 */
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* XRingMesh;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* YRingMesh;

    UPROPERTY(EditAnywhere)
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

	UPROPERTY()
	TMap<int32, UCreatorPropertyBase*> PropertyMap;

	void AddProperty(int32 id, UCreatorPropertyBase* Property);

	void SelectAxis(bool isX, bool isY, bool isZ);
	void SelectRotationAxis(bool isX, bool isY, bool isZ);
	void SelectScaleAxis(bool isX, bool isY, bool isZ);

	void Drag(FVector2D MouseDownPosition, FVector2D MousePosition);

	void DragEnd(ECreatorToolState ToolState);

	UFUNCTION()
	void OnChangePosition(FVector Pos);
	UFUNCTION()
	void OnChangeRotation(FRotator Rot);
	UFUNCTION()
	void OnChangeScale(FVector Scale);

	virtual void SetFileName(const FString& FileName);

	bool IsSelectedObject;
public:
	virtual void RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const;
	virtual void SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject);

	// 하이라키 기능
	bool IsExpandedHierarchy = true;
};
