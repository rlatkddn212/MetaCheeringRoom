// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorGizmo/CreatorRotationGizmoComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UCreatorRotationGizmoComponent::UCreatorRotationGizmoComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    // 회전 링 초기화 및 설정
    XRingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("XRingMesh"));
    XRingMesh->SetupAttachment(this);
    XRingMesh->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));  // Z축 링 회전
    XRingMesh->SetVisibility(false);

    YRingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("YRingMesh"));
    YRingMesh->SetupAttachment(this);
    YRingMesh->SetRelativeRotation(FRotator(0.f, 0.f, 90.f));  // X축 링 회전
    YRingMesh->SetVisibility(false);

    ZRingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZRingMesh"));
    ZRingMesh->SetupAttachment(this);
    ZRingMesh->SetVisibility(false);

    // 링 메쉬 설정 (Torus 사용 예시)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> RingMesh(TEXT("/Script/Engine.StaticMesh'/Game/Ksw/Ring.Ring'"));
    if (RingMesh.Succeeded())
    {
        XRingMesh->SetStaticMesh(RingMesh.Object);
        YRingMesh->SetStaticMesh(RingMesh.Object);
        ZRingMesh->SetStaticMesh(RingMesh.Object);
    }
}


// Called when the game starts
void UCreatorRotationGizmoComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UCreatorRotationGizmoComponent::SetGizmoVisibility(bool isVisible)
{
    XRingMesh->SetVisibility(isVisible);
    YRingMesh->SetVisibility(isVisible);
    ZRingMesh->SetVisibility(isVisible);
}

