// Copyright 2021 ls Sun, Inc. All Rights Reserved.


#include "FileActor.h"
#include <Components/SceneComponent.h>

UMaterialInterface* AFileActor::m_pDefaultMaterial = 0;
UMaterialInterface* AFileActor::m_pDefaultAlphaMaterial = 0;
UMaterialInterface* AFileActor::m_pDefaultColorMaterial = 0;
UMaterialInterface* AFileActor::m_pVertexColorMaterial = 0;

// Sets default values
AFileActor::AFileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    MyRoot = nullptr;
   /* USceneComponent* SceneComponent = NewObject<USceneComponent>(this, "SceneComponent");
    SetRootComponent(SceneComponent);*/

    if (!m_pDefaultMaterial)
    {
        ConstructorHelpers::FObjectFinder<UMaterialInterface> finder(TEXT("/RuntimeLoadFbx/Mats/FIO_BasicMat"));
        m_pDefaultMaterial = finder.Object;
    }
    if (!m_pDefaultAlphaMaterial)
    {
        ConstructorHelpers::FObjectFinder<UMaterialInterface> finder(TEXT("/RuntimeLoadFbx/Mats/FIO_BasicMat_Alpha"));
        m_pDefaultAlphaMaterial = finder.Object;
    }
    if (!m_pDefaultColorMaterial)
    {
        ConstructorHelpers::FObjectFinder<UMaterialInterface> finder(TEXT("/RuntimeLoadFbx/Mats/FIO_ColorMat"));
        m_pDefaultColorMaterial = finder.Object;
    }
    if (!m_pVertexColorMaterial)
    {
        ConstructorHelpers::FObjectFinder<UMaterialInterface> finder(TEXT("/RuntimeLoadFbx/Mats/FIO_VertexColorMat"));
        m_pVertexColorMaterial = finder.Object;
    }

}

// Called when the game starts or when spawned
void AFileActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (!MyRoot)
        MyRoot = RootComponent;

}

