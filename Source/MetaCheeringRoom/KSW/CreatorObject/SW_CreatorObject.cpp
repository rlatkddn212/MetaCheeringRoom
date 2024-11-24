// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorObject/SW_CreatorObject.h"
#include "KSW/CreatorStorageSubsystem.h"
#include "KSW/CreatorGizmo/CreatorScaleGizmoComponent.h"
#include "KSW/CreatorGizmo/CreatorRotationGizmoComponent.h"
#include "KSW/CreatorGizmo/CreatorPositionGizmoComponent.h"
#include "KSW/SW_CreatorPlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/UObjectGlobals.h"
#include "MeshDescription.h"
#include "Engine/StaticMesh.h"
#include "MeshTypes.h"
#include "StaticMeshResources.h"
#include "StaticMeshAttributes.h"
#include "Engine/StaticMeshSourceData.h"
#include "Chaos/Array.h"
#include "Math/Vector.h"
#include "UObject/NoExportTypes.h"
#include "MeshDescription.h"
#include "StaticMeshAttributes.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASW_CreatorObject::ASW_CreatorObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	GizmoRoot = CreateDefaultSubobject<USceneComponent>(TEXT("GizmoRoot"));
	GizmoRoot->SetupAttachment(Root);

	// 루트 컴포넌트 생성 및 설정
	// X축 메시 생성 및 초기화
	XAxisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("XAxisMesh"));
	YAxisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("YAxisMesh"));
	ZAxisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZAxisMesh"));

	XAxisMesh->ComponentTags.Add(FName("XAxisMesh"));
	YAxisMesh->ComponentTags.Add(FName("YAxisMesh"));
	ZAxisMesh->ComponentTags.Add(FName("ZAxisMesh"));

	XAxisMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	YAxisMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	ZAxisMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);

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

	XAxisMesh->SetupAttachment(GizmoRoot);
	YAxisMesh->SetupAttachment(GizmoRoot);
	ZAxisMesh->SetupAttachment(GizmoRoot);
	XAxisMesh->SetAbsolute(false, false, true);
	YAxisMesh->SetAbsolute(false, false, true);
	ZAxisMesh->SetAbsolute(false, false, true);

	// 회전 링 초기화 및 설정
	XRingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("XRingMesh"));
	XRingMesh->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));  // Z축 링 회전

	YRingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("YRingMesh"));
	YRingMesh->SetRelativeRotation(FRotator(0.f, 0.f, 90.f));  // X축 링 회전

	ZRingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZRingMesh"));

	XRingMesh->ComponentTags.Add(FName("XRingMesh"));
	YRingMesh->ComponentTags.Add(FName("YRingMesh"));
	ZRingMesh->ComponentTags.Add(FName("ZRingMesh"));

	XRingMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	YRingMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	ZRingMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);

	XRingMesh->SetAbsolute(false, false, true);
	YRingMesh->SetAbsolute(false, false, true);
	ZRingMesh->SetAbsolute(false, false, true);

	//// 링 메쉬 설정 (Torus 사용 예시)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RingMesh(TEXT("/Script/Engine.StaticMesh'/Game/Ksw/Ring.Ring'"));
	if (RingMesh.Succeeded())
	{
		XRingMesh->SetStaticMesh(RingMesh.Object);
		YRingMesh->SetStaticMesh(RingMesh.Object);
		ZRingMesh->SetStaticMesh(RingMesh.Object);
	}

	// root에 부착한다.
	XRingMesh->SetupAttachment(GizmoRoot);
	YRingMesh->SetupAttachment(GizmoRoot);
	ZRingMesh->SetupAttachment(GizmoRoot);

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

	XScaleAxisMesh->SetupAttachment(GizmoRoot);
	YScaleAxisMesh->SetupAttachment(GizmoRoot);
	ZScaleAxisMesh->SetupAttachment(GizmoRoot);

	XScaleRectMesh->SetupAttachment(XScaleAxisMesh, "Socket");
	YScaleRectMesh->SetupAttachment(YScaleAxisMesh, "Socket");
	ZScaleRectMesh->SetupAttachment(ZScaleAxisMesh, "Socket");

	XScaleAxisMesh->SetAbsolute(false, false, true);
	YScaleAxisMesh->SetAbsolute(false, false, true);
	ZScaleAxisMesh->SetAbsolute(false, false, true);

	XYAxisMeshX = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("XYAxisMeshX"));
	XYAxisMeshY = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("XYAxisMeshY"));
	
	XZAxisMeshX = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("XZAxisMeshX"));
	XZAxisMeshZ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("XZAxisMeshZ"));

	YZAxisMeshY = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("YZAxisMeshY"));
	YZAxisMeshZ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("YZAxisMeshZ"));

	XYAxisMeshX->ComponentTags.Add(FName("XYAxisMesh"));
	XYAxisMeshY->ComponentTags.Add(FName("XYAxisMesh"));

	XZAxisMeshX->ComponentTags.Add(FName("XZAxisMesh"));
	XZAxisMeshZ->ComponentTags.Add(FName("XZAxisMesh"));

	YZAxisMeshY->ComponentTags.Add(FName("YZAxisMesh"));
	YZAxisMeshZ->ComponentTags.Add(FName("YZAxisMesh"));

	XYAxisMeshX->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	XYAxisMeshY->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);

	XZAxisMeshX->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	XZAxisMeshZ->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);

	YZAxisMeshY->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	YZAxisMeshZ->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> AxisMesh(TEXT("/Script/Engine.StaticMesh'/Game/Ksw/BoundingBoxEdge.BoundingBoxEdge'"));

	if (ScaleAxisMesh.Succeeded())
	{
		XYAxisMeshX->SetStaticMesh(ScaleAxisMesh.Object);
		XYAxisMeshY->SetStaticMesh(ScaleAxisMesh.Object);

		XZAxisMeshX->SetStaticMesh(ScaleAxisMesh.Object);
		XZAxisMeshZ->SetStaticMesh(ScaleAxisMesh.Object);

		YZAxisMeshY->SetStaticMesh(ScaleAxisMesh.Object);
		YZAxisMeshZ->SetStaticMesh(ScaleAxisMesh.Object);
	}


	//(Pitch=0.000000,Yaw=90.000000,Roll=0.000000)
	XYAxisMeshX->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	XYAxisMeshX->SetRelativeScale3D(FVector(0.0011f, 1.5f, 1.5f));

	//(X=7.000000,Y=11.000000,Z=0.000000)
	//(Pitch=0.000000,Yaw=0.000000,Roll=0.000000)
	//(X=1.000000,Y=2.000000,Z=2.000000)
	XYAxisMeshY->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	XYAxisMeshY->SetRelativeScale3D(FVector(0.0011f, 1.5f, 1.5f));

	// (X=11.000000,Y=0.000000,Z=7.000000)
	// (Pitch=90.000000,Yaw=0.000000,Roll=0.000000)
	XZAxisMeshX->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	XZAxisMeshX->SetRelativeScale3D(FVector(0.0011f, 1.5f, 1.5f));
	
	// (X=7.000000,Y=0.000000,Z=11.000000)
	XZAxisMeshZ->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	XZAxisMeshZ->SetRelativeScale3D(FVector(0.0011f, 1.5f, 1.5f));

	// (X=0.000000,Y=11.000000,Z=7.000000)
	// (Pitch=90.000000,Yaw=0.000000,Roll=0.000000)
	YZAxisMeshY->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	YZAxisMeshY->SetRelativeScale3D(FVector(0.0011f, 1.5f, 1.5f));

	// (X=0.000000,Y=7.000000,Z=11.000000)
	// (Pitch=0.000000,Yaw=90.000000,Roll=0.000000)
	YZAxisMeshZ->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	YZAxisMeshZ->SetRelativeScale3D(FVector(0.0011f, 1.5f, 1.5f));
	
	XYAxisMeshX->SetupAttachment(XAxisMesh, "Socket");
	XYAxisMeshY->SetupAttachment(YAxisMesh, "Socket");

	XZAxisMeshX->SetupAttachment(XAxisMesh, "Socket");
	XZAxisMeshZ->SetupAttachment(ZAxisMesh, "Socket");

	YZAxisMeshY->SetupAttachment(YAxisMesh, "Socket");
	YZAxisMeshZ->SetupAttachment(ZAxisMesh, "Socket");

	// 기즈모 컴포넌트 생성 및 설정
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

	ChangeToolMode(ECreatorToolState::Selection);
}

// Called every frame
void ASW_CreatorObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsSelectedObject)
	{
		FVector Target = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();

		// Target과의 거리가 멀수록 HP바가 커진다.
		float Distance = FVector::Distance(Target, GetActorLocation());
		float Scale = Distance / 750;

		SetGizmoSize(Scale * 2.0f);
	}
}

void ASW_CreatorObject::RecursiveDestroy(AActor* Actor)
{
	TArray<AActor*> AttachedActors;
	Actor->GetAttachedActors(AttachedActors);

	for (AActor* AttachedActor : AttachedActors)
	{
		if (AttachedActor && !AttachedActor->IsPendingKillPending())
		{
			RecursiveDestroy(AttachedActor);
		}
	}

	Actor->Destroy();
}

void ASW_CreatorObject::Destroyed()
{
	Super::Destroyed();
	
	// Attach된 Actor 삭제
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);

	for (AActor* AttachedActor : AttachedActors)
	{
		if (AttachedActor && !AttachedActor->IsPendingKillPending())
		{
			RecursiveDestroy(AttachedActor);
		}
	}
}	

void ASW_CreatorObject::OnSelected(bool isSelected)
{
	IsSelectedObject = isSelected;
	if (!isSelected)
	{
		ChangeToolMode(ECreatorToolState::Selection);
	}

	OnChangeSelected.ExecuteIfBound(isSelected);
}

void ASW_CreatorObject::OnChangeProperty(int32 id, UCreatorPropertyBase* CreatorProperty)
{
	
}

void ASW_CreatorObject::DoDestroy()
{
	Destroy();
}

void ASW_CreatorObject::SetGizmoSize(float size)
{
	XAxisMesh->SetRelativeScale3D(FVector(1.0f * size, 1.0f * size, 1.0f * size));
	YAxisMesh->SetRelativeScale3D(FVector(1.0f * size, 1.0f * size, 1.0f * size));
	ZAxisMesh->SetRelativeScale3D(FVector(1.0f * size, 1.0f * size, 1.0f * size));

	XRingMesh->SetRelativeScale3D(FVector(1.0f * size, 1.0f * size, 1.0f * size));
	YRingMesh->SetRelativeScale3D(FVector(1.0f * size, 1.0f * size, 1.0f * size));
	ZRingMesh->SetRelativeScale3D(FVector(1.0f * size, 1.0f * size, 1.0f * size));

	XScaleAxisMesh->SetRelativeScale3D(FVector(0.004f * size, 4.0f * size, 4.0f * size));
	YScaleAxisMesh->SetRelativeScale3D(FVector(0.004f * size, 4.0f * size, 4.0f * size));
	ZScaleAxisMesh->SetRelativeScale3D(FVector(0.004f * size, 4.0f * size, 4.0f * size));
}

void ASW_CreatorObject::ChangeToolMode(ECreatorToolState state)
{
	PositionGizmo->SetAxisSelected(false, false, false);
	RotationGizmo->SetAxisSelected(false, false, false);
	ScaleGizmo->SetAxisSelected(false, false, false);

	// hidden in game
	XAxisMesh->SetHiddenInGame(true);
	YAxisMesh->SetHiddenInGame(true);
	ZAxisMesh->SetHiddenInGame(true);
	XAxisMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	YAxisMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ZAxisMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	XYAxisMeshX->SetHiddenInGame(true);
	XYAxisMeshY->SetHiddenInGame(true);
	XZAxisMeshX->SetHiddenInGame(true);
	XZAxisMeshZ->SetHiddenInGame(true);
	YZAxisMeshY->SetHiddenInGame(true);
	YZAxisMeshZ->SetHiddenInGame(true);

	XYAxisMeshX->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	XYAxisMeshY->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	XZAxisMeshX->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	XZAxisMeshZ->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	YZAxisMeshY->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	YZAxisMeshZ->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

		XYAxisMeshX->SetHiddenInGame(false);
		XYAxisMeshY->SetHiddenInGame(false);
		XZAxisMeshX->SetHiddenInGame(false);
		XZAxisMeshZ->SetHiddenInGame(false);
		YZAxisMeshY->SetHiddenInGame(false);
		YZAxisMeshZ->SetHiddenInGame(false);
		XYAxisMeshX->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		XYAxisMeshY->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		XZAxisMeshX->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		XZAxisMeshZ->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		YZAxisMeshY->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		YZAxisMeshZ->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
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

void ASW_CreatorObject::Server_SetLocation_Implementation(FVector Pos)
{
	SetActorLocation(Pos);
}

void ASW_CreatorObject::Server_SetRotation_Implementation(FQuat Rot)
{
	SetActorRotation(Rot);
}

void ASW_CreatorObject::Server_SetScale_Implementation(FVector Scale)
{
	//SetActorScale3D(Scale);

	// 멀티케스트
	Multicast_SetScale(Scale);
}

void ASW_CreatorObject::Multicast_SetScale_Implementation(FVector Scale)
{
	SetActorScale3D(Scale);
}

void ASW_CreatorObject::AddProperty(int32 id, UCreatorPropertyBase* Property)
{
	if (PropertyMap.Contains(id))
	{
		PropertyMap[id] = Property;
	}
	else
	{
		PropertyMap.Add(id, Property);
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

void ASW_CreatorObject::Drag(FVector2D MouseDownPosition, FVector2D MousePosition)
{
	ASW_CreatorPlayerController* PC = Cast<ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		ECreatorToolState ToolState = PC->GetToolState();
		switch (ToolState)
		{
		case ECreatorToolState::Position:
			PositionGizmo->Drag(MouseDownPosition, MousePosition);
			break;
		case ECreatorToolState::Rotation:
			RotationGizmo->Drag(MouseDownPosition, MousePosition);
			break;
		case ECreatorToolState::Scale:
			ScaleGizmo->Drag(MouseDownPosition, MousePosition);
			break;
		}
	}
	
}

void ASW_CreatorObject::DragEnd(ECreatorToolState ToolState)
{
	switch (ToolState)
	{
	case ECreatorToolState::Position:
		PositionGizmo->SetAxisSelected(false, false, false);
		break;
	case ECreatorToolState::Rotation:
		RotationGizmo->SetAxisSelected(false, false, false);
		break;
	case ECreatorToolState::Scale:
		ScaleGizmo->SetAxisSelected(false, false, false);
		break;
	}
}

void ASW_CreatorObject::OnChangePosition(FVector Pos)
{
	Server_SetLocation(Pos);
}

void ASW_CreatorObject::OnChangeRotation(FRotator Rot)
{
	Server_SetRotation(Rot.Quaternion());
}

void ASW_CreatorObject::OnChangeScale(FVector Scale)
{
	Server_SetScale(Scale);
}

void ASW_CreatorObject::SetFileName(const FString& FileName)
{
	// FBX에서 사용한다.
}

void ASW_CreatorObject::SetCreatorObjectName(const FString& Name)
{
	CreatorObjectName = FName(Name);
}

FString ASW_CreatorObject::GetCreatorObjectName() const
{
	return CreatorObjectName.ToString();
}

void ASW_CreatorObject::RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const
{
	
}

void ASW_CreatorObject::SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject)
{
}

void ASW_CreatorObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASW_CreatorObject, CreatorObjectName);
}
