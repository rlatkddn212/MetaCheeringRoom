// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CreatorRotationGizmoComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class METACHEERINGROOM_API UCreatorRotationGizmoComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCreatorRotationGizmoComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
		
    /** X, Y, Z 회전용 링 메쉬 */
    UPROPERTY(VisibleAnywhere, Category = "Gizmo")
    UStaticMeshComponent* XRingMesh;

    UPROPERTY(VisibleAnywhere, Category = "Gizmo")
    UStaticMeshComponent* YRingMesh;

    UPROPERTY(VisibleAnywhere, Category = "Gizmo")
    UStaticMeshComponent* ZRingMesh;

    /** 기즈모 표시/숨김 설정 */
    UFUNCTION(BlueprintCallable)
    void SetGizmoVisibility(bool isVisible);
};
