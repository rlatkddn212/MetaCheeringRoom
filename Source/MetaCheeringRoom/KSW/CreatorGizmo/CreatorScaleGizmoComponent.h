// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CreatorScaleGizmoComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class METACHEERINGROOM_API UCreatorScaleGizmoComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCreatorScaleGizmoComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		   
	/** 기즈모 활성화 함수 */
    UFUNCTION(BlueprintCallable)
	void SetGizmoVisibility(bool isVisible);
	// 축 선택
	void SetAxisSelected(bool isX, bool isY, bool isZ);

	// 선택한 축 정보 저장
	UPROPERTY()
	bool IsXAxisSelected;
	UPROPERTY()
	bool IsYAxisSelected;
	UPROPERTY()
	bool IsZAxisSelected;

	void Drag(FVector2D MouseDownPosition, FVector2D MousePosition);
	FVector OnMouseClick(const FVector2D& ScreenPosition);
	void GetClickRay(const APlayerController* PlayerController, const FVector2D& ScreenPosition, FVector& RayOrigin, FVector& RayDirection);

	FVector ClosestPointOnAxisToRay(const FVector& LinePoint, const FVector& LineDirection, const FVector& RayOrigin, const FVector& RayDirection);

	// 머터리얼 정보
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
	FVector GizmoStartScale;

	UPROPERTY()
	float Factor = 0.025f;
};
