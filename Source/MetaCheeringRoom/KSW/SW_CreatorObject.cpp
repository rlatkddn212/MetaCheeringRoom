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

	// 루트 컴포넌트 생성 및 설정
	// X축 메시 생성 및 초기화
	XAxisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("XAxisMesh"));
	YAxisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("YAxisMesh"));
	ZAxisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZAxisMesh"));

	// 기본 메시 설정 (옵션)
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

	// 회전 링 초기화 및 설정
	XRingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("XRingMesh"));
	XRingMesh->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));  // Z축 링 회전

	YRingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("YRingMesh"));
	YRingMesh->SetRelativeRotation(FRotator(0.f, 0.f, 90.f));  // X축 링 회전

	ZRingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZRingMesh"));

	// 링 메쉬 설정 (Torus 사용 예시)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RingMesh(TEXT("/Script/Engine.StaticMesh'/Game/Ksw/Ring.Ring'"));
	if (RingMesh.Succeeded())
	{
		XRingMesh->SetStaticMesh(RingMesh.Object);
		YRingMesh->SetStaticMesh(RingMesh.Object);
		ZRingMesh->SetStaticMesh(RingMesh.Object);
	}

	// root에 부착한다.
	XRingMesh->SetupAttachment(Root);
	YRingMesh->SetupAttachment(Root);
	ZRingMesh->SetupAttachment(Root);

	XScaleAxisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("XScaleAxisMesh"));
	YScaleAxisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("YScaleAxisMesh"));
	ZScaleAxisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZScaleAxisMesh"));

	XScaleRectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("XScaleRectMesh"));
	YScaleRectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("YScaleRectMesh"));
	ZScaleRectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZScaleRectMesh"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ScaleAxisMesh(TEXT("/Script/Engine.StaticMesh'/Game/Ksw/TranslateHandleLong.TranslateHandleLong'"));

	if (ScaleAxisMesh.Succeeded())
	{
		XScaleAxisMesh->SetStaticMesh(ScaleAxisMesh.Object);
		YScaleAxisMesh->SetStaticMesh(ScaleAxisMesh.Object);
		ZScaleAxisMesh->SetStaticMesh(ScaleAxisMesh.Object);
	}

	// 기본 메시 설정 (옵션)
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
}

// Called when the game starts or when spawned
void ASW_CreatorObject::BeginPlay()
{
	Super::BeginPlay();

	// 부모 스케일을 상속하지 않도록 설정
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
	XAxisMesh->SetVisibility(false);
	YAxisMesh->SetVisibility(false);
	ZAxisMesh->SetVisibility(false);

	XRingMesh->SetVisibility(false);
	YRingMesh->SetVisibility(false);
	ZRingMesh->SetVisibility(false);

	XScaleAxisMesh->SetVisibility(false);
	YScaleAxisMesh->SetVisibility(false);
	ZScaleAxisMesh->SetVisibility(false);

	XScaleRectMesh->SetVisibility(false);
	YScaleRectMesh->SetVisibility(false);
	ZScaleRectMesh->SetVisibility(false);

	switch (state)
	{
	case ECreatorToolState::Selection:
		break;
	case ECreatorToolState::Position:
		XAxisMesh->SetVisibility(true);
		YAxisMesh->SetVisibility(true);
		ZAxisMesh->SetVisibility(true);
		break;
	case ECreatorToolState::Rotation:
		XRingMesh->SetVisibility(true);
		YRingMesh->SetVisibility(true);
		ZRingMesh->SetVisibility(true);
		break;
	case ECreatorToolState::Scale:
		XScaleAxisMesh->SetVisibility(true);
		YScaleAxisMesh->SetVisibility(true);
		ZScaleAxisMesh->SetVisibility(true);

		XScaleRectMesh->SetVisibility(true);
		YScaleRectMesh->SetVisibility(true);
		ZScaleRectMesh->SetVisibility(true);
		break;
	}
}

