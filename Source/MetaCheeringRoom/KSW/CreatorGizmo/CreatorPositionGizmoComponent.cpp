// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorGizmo/CreatorPositionGizmoComponent.h"

// Sets default values for this component's properties
UCreatorPositionGizmoComponent::UCreatorPositionGizmoComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    // X축 메시 생성 및 초기화
    XAxisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("XAxisMesh"));
    XAxisMesh->SetupAttachment(this);
    XAxisMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
    // 회전
    XAxisMesh->SetVisibility(false);

    // Y축 메시 생성 및 초기화
    YAxisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("YAxisMesh"));
    YAxisMesh->SetupAttachment(this);
    YAxisMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

    YAxisMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
    // 회전
    YAxisMesh->SetVisibility(false);

    // Z축 메시 생성 및 초기화
    ZAxisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZAxisMesh"));
    ZAxisMesh->SetupAttachment(this);
    ZAxisMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
    // 회전
    ZAxisMesh->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
    ZAxisMesh->SetVisibility(false);

    // 기본 메시 설정 (옵션)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Script/Engine.StaticMesh'/Game/Ksw/TranslateArrowHandle.TranslateArrowHandle'"));
    if (CylinderMesh.Succeeded())
    {
        XAxisMesh->SetStaticMesh(CylinderMesh.Object);
        YAxisMesh->SetStaticMesh(CylinderMesh.Object);
        ZAxisMesh->SetStaticMesh(CylinderMesh.Object);
    }
}


// Called when the game starts
void UCreatorPositionGizmoComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UCreatorPositionGizmoComponent::OnComponentCreated()
{
    Super::OnComponentCreated();

    // 컴포넌트 생성 시 기즈모 숨김 처리
    SetGizmoVisibility(true);
}

void UCreatorPositionGizmoComponent::SetGizmoVisibility(bool isVisible)
{
    XAxisMesh->SetVisibility(isVisible);
    YAxisMesh->SetVisibility(isVisible);
    ZAxisMesh->SetVisibility(isVisible);
}

