// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorGizmo/CreatorPositionGizmoComponent.h"

// Sets default values for this component's properties
UCreatorPositionGizmoComponent::UCreatorPositionGizmoComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    // X�� �޽� ���� �� �ʱ�ȭ
    XAxisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("XAxisMesh"));
    XAxisMesh->SetupAttachment(this);
    XAxisMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
    // ȸ��
    XAxisMesh->SetVisibility(false);

    // Y�� �޽� ���� �� �ʱ�ȭ
    YAxisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("YAxisMesh"));
    YAxisMesh->SetupAttachment(this);
    YAxisMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

    YAxisMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
    // ȸ��
    YAxisMesh->SetVisibility(false);

    // Z�� �޽� ���� �� �ʱ�ȭ
    ZAxisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZAxisMesh"));
    ZAxisMesh->SetupAttachment(this);
    ZAxisMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
    // ȸ��
    ZAxisMesh->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
    ZAxisMesh->SetVisibility(false);

    // �⺻ �޽� ���� (�ɼ�)
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

    // ������Ʈ ���� �� ����� ���� ó��
    SetGizmoVisibility(true);
}

void UCreatorPositionGizmoComponent::SetGizmoVisibility(bool isVisible)
{
    XAxisMesh->SetVisibility(isVisible);
    YAxisMesh->SetVisibility(isVisible);
    ZAxisMesh->SetVisibility(isVisible);
}

