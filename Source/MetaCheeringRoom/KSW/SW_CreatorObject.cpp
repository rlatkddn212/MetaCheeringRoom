// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/SW_CreatorObject.h"
#include "CreatorStorageSubsystem.h"
#include "CreatorGizmo/CreatorScaleGizmoComponent.h"
#include "CreatorGizmo/CreatorRotationGizmoComponent.h"
#include "CreatorGizmo/CreatorPositionGizmoComponent.h"
#include "SW_CreatorPlayerController.h"

// Sets default values
ASW_CreatorObject::ASW_CreatorObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// ��Ʈ ������Ʈ ���� �� ����
	// X�� �޽� ���� �� �ʱ�ȭ
	XAxisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("XAxisMesh"));
	YAxisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("YAxisMesh"));
	ZAxisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZAxisMesh"));

	XAxisMesh->ComponentTags.Add(FName("XAxisMesh"));
	YAxisMesh->ComponentTags.Add(FName("YAxisMesh"));
	ZAxisMesh->ComponentTags.Add(FName("ZAxisMesh"));

	XAxisMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	YAxisMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	ZAxisMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);

	// �⺻ �޽� ���� (�ɼ�)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Script/Engine.StaticMesh'/Game/Ksw/TranslateArrowHandle.TranslateArrowHandle'"));
	if (CylinderMesh.Succeeded())
	{
		XAxisMesh->SetStaticMesh(CylinderMesh.Object);
		YAxisMesh->SetStaticMesh(CylinderMesh.Object);
		ZAxisMesh->SetStaticMesh(CylinderMesh.Object);
	}

	XAxisMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

	YAxisMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	YAxisMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	ZAxisMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	ZAxisMesh->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

	XAxisMesh->SetupAttachment(Root);
	YAxisMesh->SetupAttachment(Root);
	ZAxisMesh->SetupAttachment(Root);

	// ȸ�� �� �ʱ�ȭ �� ����
	XRingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("XRingMesh"));
	XRingMesh->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));  // Z�� �� ȸ��

	YRingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("YRingMesh"));
	YRingMesh->SetRelativeRotation(FRotator(0.f, 0.f, 90.f));  // X�� �� ȸ��

	ZRingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZRingMesh"));

	XRingMesh->ComponentTags.Add(FName("XRingMesh"));
	YRingMesh->ComponentTags.Add(FName("YRingMesh"));
	ZRingMesh->ComponentTags.Add(FName("ZRingMesh"));

	XRingMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	YRingMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	ZRingMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);

	// �� �޽� ���� (Torus ��� ����)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RingMesh(TEXT("/Script/Engine.StaticMesh'/Game/Ksw/Ring.Ring'"));
	if (RingMesh.Succeeded())
	{
		XRingMesh->SetStaticMesh(RingMesh.Object);
		YRingMesh->SetStaticMesh(RingMesh.Object);
		ZRingMesh->SetStaticMesh(RingMesh.Object);
	}

	// root�� �����Ѵ�.
	XRingMesh->SetupAttachment(Root);
	YRingMesh->SetupAttachment(Root);
	ZRingMesh->SetupAttachment(Root);

	XScaleAxisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("XScaleAxisMesh"));
	YScaleAxisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("YScaleAxisMesh"));
	ZScaleAxisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZScaleAxisMesh"));

	XScaleRectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("XScaleRectMesh"));
	YScaleRectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("YScaleRectMesh"));
	ZScaleRectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZScaleRectMesh"));

	XScaleAxisMesh->ComponentTags.Add(FName("XScaleMesh"));
	YScaleAxisMesh->ComponentTags.Add(FName("YScaleMesh"));
	ZScaleAxisMesh->ComponentTags.Add(FName("ZScaleMesh"));

	XScaleRectMesh->ComponentTags.Add(FName("XScaleMesh"));
	YScaleRectMesh->ComponentTags.Add(FName("YScaleMesh"));
	ZScaleRectMesh->ComponentTags.Add(FName("ZScaleMesh"));

	XScaleAxisMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	YScaleAxisMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	ZScaleAxisMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	XScaleRectMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	YScaleRectMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	ZScaleRectMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ScaleAxisMesh(TEXT("/Script/Engine.StaticMesh'/Game/Ksw/TranslateHandleLong.TranslateHandleLong'"));

	if (ScaleAxisMesh.Succeeded())
	{
		XScaleAxisMesh->SetStaticMesh(ScaleAxisMesh.Object);
		YScaleAxisMesh->SetStaticMesh(ScaleAxisMesh.Object);
		ZScaleAxisMesh->SetStaticMesh(ScaleAxisMesh.Object);
	}

	// �⺻ �޽� ���� (�ɼ�)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RectMesh(TEXT("/Script/Engine.StaticMesh'/Game/Ksw/UniformScaleHandle.UniformScaleHandle'"));

	if (RectMesh.Succeeded())
	{
		XScaleRectMesh->SetStaticMesh(RectMesh.Object);
		YScaleRectMesh->SetStaticMesh(RectMesh.Object);
		ZScaleRectMesh->SetStaticMesh(RectMesh.Object);
	}

	// (X=0.004000,Y=4.000000,Z=4.000000)
	
	XScaleAxisMesh->SetRelativeScale3D(FVector(0.004f, 4.0f, 4.0f));

	YScaleAxisMesh->SetRelativeScale3D(FVector(0.004f, 4.0f, 4.0f));
	YScaleAxisMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	ZScaleAxisMesh->SetRelativeScale3D(FVector(0.004f, 4.0f, 4.0f));
	ZScaleAxisMesh->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

	XScaleRectMesh->SetRelativeScale3D(FVector(250.0f, 0.25f, 0.25f));
	YScaleRectMesh->SetRelativeScale3D(FVector(250.0f, 0.25f, 0.25f));
	ZScaleRectMesh->SetRelativeScale3D(FVector(250.0f, 0.25f, 0.25f));

	XScaleAxisMesh->SetupAttachment(Root);
	YScaleAxisMesh->SetupAttachment(Root);
	ZScaleAxisMesh->SetupAttachment(Root);

	XScaleRectMesh->SetupAttachment(XScaleAxisMesh, "Socket");
	YScaleRectMesh->SetupAttachment(YScaleAxisMesh, "Socket");
	ZScaleRectMesh->SetupAttachment(ZScaleAxisMesh, "Socket");

	PositionGizmo = CreateDefaultSubobject<UCreatorPositionGizmoComponent>(TEXT("PositionGizmo"));
	RotationGizmo = CreateDefaultSubobject<UCreatorRotationGizmoComponent>(TEXT("RotationGizmo"));
	ScaleGizmo = CreateDefaultSubobject<UCreatorScaleGizmoComponent>(TEXT("ScaleGizmo"));

	PositionGizmo->SetupAttachment(Root);
	RotationGizmo->SetupAttachment(Root);
	ScaleGizmo->SetupAttachment(Root);

	ChangeToolMode(ECreatorToolState::Selection);
}

// Called when the game starts or when spawned
void ASW_CreatorObject::BeginPlay()
{
	Super::BeginPlay();

	// �θ� �������� ������� �ʵ��� ����
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		false);

	/*XScaleRectMesh->AttachToComponent(XScaleAxisMesh, AttachmentRules);
	YScaleRectMesh->AttachToComponent(YScaleAxisMesh, AttachmentRules);
	ZScaleRectMesh->AttachToComponent(ZScaleAxisMesh, AttachmentRules);*/
}

// Called every frame
void ASW_CreatorObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASW_CreatorObject::OnSelected(bool isSelected)
{
	if (!isSelected)
	{
		ChangeToolMode(ECreatorToolState::Selection);
	}
}

void ASW_CreatorObject::DoDestroy()
{
	Destroy();
}

void ASW_CreatorObject::ChangeToolMode(ECreatorToolState state)
{
	// hidden in game
	XAxisMesh->SetHiddenInGame(true);
	YAxisMesh->SetHiddenInGame(true);
	ZAxisMesh->SetHiddenInGame(true);
	XAxisMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	YAxisMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ZAxisMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	XRingMesh->SetHiddenInGame(true);
	YRingMesh->SetHiddenInGame(true);
	ZRingMesh->SetHiddenInGame(true);
	XRingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	YRingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ZRingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	XScaleAxisMesh->SetHiddenInGame(true);
	YScaleAxisMesh->SetHiddenInGame(true);
	ZScaleAxisMesh->SetHiddenInGame(true);
	
	XScaleRectMesh->SetHiddenInGame(true);
	YScaleRectMesh->SetHiddenInGame(true);
	ZScaleRectMesh->SetHiddenInGame(true);

	XScaleAxisMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	YScaleAxisMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ZScaleAxisMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	XScaleRectMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	YScaleRectMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ZScaleRectMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	switch (state)
	{
	case ECreatorToolState::Selection:
		break;
	case ECreatorToolState::Position:
		XAxisMesh->SetHiddenInGame(false);
		YAxisMesh->SetHiddenInGame(false);
		ZAxisMesh->SetHiddenInGame(false);
		XAxisMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		YAxisMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ZAxisMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	case ECreatorToolState::Rotation:
		
		XRingMesh->SetHiddenInGame(false);
		YRingMesh->SetHiddenInGame(false);
		ZRingMesh->SetHiddenInGame(false);
		XRingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		YRingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ZRingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	case ECreatorToolState::Scale:
		XScaleAxisMesh->SetHiddenInGame(false);
		YScaleAxisMesh->SetHiddenInGame(false);
		ZScaleAxisMesh->SetHiddenInGame(false);
		XScaleAxisMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		YScaleAxisMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ZScaleAxisMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		XScaleRectMesh->SetHiddenInGame(false);
		YScaleRectMesh->SetHiddenInGame(false);
		ZScaleRectMesh->SetHiddenInGame(false);
		XScaleRectMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		YScaleRectMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ZScaleRectMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	}
}

void ASW_CreatorObject::SelectAxis(bool isX, bool isY, bool isZ)
{
	PositionGizmo->SetAxisSelected(isX, isY, isZ);
}

void ASW_CreatorObject::SelectRotationAxis(bool isX, bool isY, bool isZ)
{
	RotationGizmo->SetAxisSelected(isX, isY, isZ);
}

void ASW_CreatorObject::SelectScaleAxis(bool isX, bool isY, bool isZ)
{
	ScaleGizmo->SetAxisSelected(isX, isY, isZ);
}

