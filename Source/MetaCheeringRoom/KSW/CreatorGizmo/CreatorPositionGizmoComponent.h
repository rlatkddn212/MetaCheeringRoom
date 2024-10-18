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
	// Called every frame
	virtual void OnComponentCreated() override;
		
    /** �� �޽ø� ���� StaticMeshComponent�� */
    UPROPERTY(VisibleAnywhere, Category = "Gizmo")
    UStaticMeshComponent* XAxisMesh;

    UPROPERTY(VisibleAnywhere, Category = "Gizmo")
    UStaticMeshComponent* YAxisMesh;

    UPROPERTY(VisibleAnywhere, Category = "Gizmo")
    UStaticMeshComponent* ZAxisMesh;

	/** ����� Ȱ��ȭ �Լ� */
    UFUNCTION(BlueprintCallable)
	void SetGizmoVisibility(bool isVisible);
};
