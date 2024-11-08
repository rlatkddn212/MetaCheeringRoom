// Copyright 2021 ls Sun, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"
#include <Materials/MaterialInterface.h>
#include <Materials/MaterialInstanceDynamic.h>
#include "FileActor.generated.h"

UCLASS()
class AFileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFileActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    static UMaterialInterface* m_pDefaultMaterial;
    static UMaterialInterface* m_pDefaultAlphaMaterial;
    static UMaterialInterface* m_pDefaultColorMaterial;
    static UMaterialInterface* m_pVertexColorMaterial;

	UPROPERTY(EditAnywhere, Category = "Root")
    USceneComponent *MyRoot;
};
