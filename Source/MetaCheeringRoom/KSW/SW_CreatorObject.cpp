// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/SW_CreatorObject.h"
#include "CreatorStorageSubsystem.h"
#include "CreatorGizmo/CreatorScaleGizmoComponent.h"
#include "CreatorGizmo/CreatorRotationGizmoComponent.h"
#include "CreatorGizmo/CreatorPositionGizmoComponent.h"
#include "SW_CreatorPlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/UObjectGlobals.h"
#include "MeshDescription.h"
#include "Engine/StaticMesh.h"
#include "MeshTypes.h"
#include "StaticMeshResources.h"
#include "StaticMeshAttributes.h"
#include "Engine/StaticMeshSourceData.h"
#include "RawMesh.h"
#include "Chaos/Array.h"
#include "Math/Vector.h"
#include "UObject/NoExportTypes.h"
#include "MeshDescription.h"
#include "StaticMeshAttributes.h"


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

	XAxisMesh->SetupAttachment(Root);
	YAxisMesh->SetupAttachment(Root);
	ZAxisMesh->SetupAttachment(Root);
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

	//if (UStaticMesh* NewMesh = CreateCylinderMesh(50.0f, 3.0f, 16))
	//{
	//	XRingMesh->SetStaticMesh(NewMesh);
	//	YRingMesh->SetStaticMesh(NewMesh);
	//	ZRingMesh->SetStaticMesh(NewMesh);
	//}

	//// 링 메쉬 설정 (Torus 사용 예시)
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

	XScaleAxisMesh->SetupAttachment(Root);
	YScaleAxisMesh->SetupAttachment(Root);
	ZScaleAxisMesh->SetupAttachment(Root);

	XScaleRectMesh->SetupAttachment(XScaleAxisMesh, "Socket");
	YScaleRectMesh->SetupAttachment(YScaleAxisMesh, "Socket");
	ZScaleRectMesh->SetupAttachment(ZScaleAxisMesh, "Socket");

	XScaleAxisMesh->SetAbsolute(false, false, true);
	YScaleAxisMesh->SetAbsolute(false, false, true);
	ZScaleAxisMesh->SetAbsolute(false, false, true);

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
}

void ASW_CreatorObject::OnSelected(bool isSelected)
{
	if (!isSelected)
	{
		ChangeToolMode(ECreatorToolState::Selection);
	}

	OnChangeSelected.ExecuteIfBound(isSelected);
}


void ASW_CreatorObject::DoDestroy()
{
	Destroy();
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


UStaticMesh* ASW_CreatorObject::CreateCylinderMesh(float Radius, float Height, int32 SegmentCount)
{
	// 메시 생성을 위한 기본 설정
	FString MeshName = FString::Printf(TEXT("SM_Cylinder_%d"), FMath::Rand());
	UStaticMesh* StaticMesh = NewObject<UStaticMesh>(GetTransientPackage(), *MeshName);

	// MeshDescription 생성
	FMeshDescription MeshDescription;

	FStaticMeshAttributes Attributes(MeshDescription);
	Attributes.Register();
	TVertexAttributesRef<FVector3f> VertexPositions = Attributes.GetVertexPositions();
	TVertexInstanceAttributesRef<FVector2f> VertexInstanceUVs = Attributes.GetVertexInstanceUVs();

	// 정점과 폴리곤 그룹 배열
	TArray<FVertexID> VertexIDs;
	FPolygonGroupID PolygonGroup = MeshDescription.CreatePolygonGroup();

	// 원통 측면의 정점들 생성
	for (int32 i = 0; i < SegmentCount; ++i)
	{
		float Angle = (2.0f * PI * i) / SegmentCount;
		float X = Radius * FMath::Cos(Angle);
		float Y = Radius * FMath::Sin(Angle);

		// 상단 정점
		FVertexID TopVertex = MeshDescription.CreateVertex();
		VertexPositions[TopVertex] = FVector3f(X, Y, Height * 0.5f);
		VertexIDs.Add(TopVertex);

		// 하단 정점
		FVertexID BottomVertex = MeshDescription.CreateVertex();
		VertexPositions[BottomVertex] = FVector3f(X, Y, -Height * 0.5f);
		VertexIDs.Add(BottomVertex);
	}

	// 삼각형 생성 함수
	auto CreateTriangle = [&](FVertexID V0, FVertexID V1, FVertexID V2, bool bReverse = true)
		{
			TArray<FVertexInstanceID> VertexInstances;
			TArray<FVertexID> TriVertices;
			if (!bReverse)
			{
				TriVertices = {V0, V1, V2};
			}
			else
			{
				TriVertices = {V0, V2, V1};
			}

			for (FVertexID VertexID : TriVertices)
			{
				FVertexInstanceID InstanceID = MeshDescription.CreateVertexInstance(VertexID);
				VertexInstances.Add(InstanceID);

				// UV 좌표 계산
				FVector3f Position = VertexPositions[VertexID];
				float U = FMath::Atan2(Position.Y, Position.X) / (2.0f * PI) + 0.5f;
				float V = (Position.Z + Height * 0.5f) / Height;
				VertexInstanceUVs.Set(InstanceID, 0, FVector2f(U, V));
			}

			MeshDescription.CreateTriangle(PolygonGroup, VertexInstances);
		};

	// 측면 삼각형 생성
	for (int32 i = 0; i < SegmentCount; ++i)
	{
		int32 Current = i * 2;
		int32 Next = ((i + 1) % SegmentCount) * 2;
		CreateTriangle(VertexIDs[Current], VertexIDs[Current + 1], VertexIDs[Next]);
		CreateTriangle(VertexIDs[Next], VertexIDs[Current + 1], VertexIDs[Next + 1]);
	}

	UStaticMesh::FBuildMeshDescriptionsParams Params;
	Params.bBuildSimpleCollision = true;
	StaticMesh->NaniteSettings.bEnabled = true;
	StaticMesh->BuildFromMeshDescriptions({&MeshDescription}, Params);

	// StaticMesh 생성 완료
	StaticMesh->CommitMeshDescription(0);
	StaticMesh->SetLightingGuid();

	// 메시 설정
	StaticMesh->CreateBodySetup();

	// 빌드 설정
	UStaticMesh::FBuildParameters BuildParams;
	StaticMesh->Build(BuildParams);
	StaticMesh->PostEditChange();

	return StaticMesh;
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
	SetActorLocation(Pos);
}

void ASW_CreatorObject::OnChangeRotation(FRotator Rot)
{
	SetActorRotation(Rot);
}

void ASW_CreatorObject::OnChangeScale(FVector Scale)
{
	SetActorScale3D(Scale);
}

