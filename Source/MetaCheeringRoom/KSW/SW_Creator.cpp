// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/SW_Creator.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "SW_CreatorPlayerController.h"
#include "UI/SW_CameraSpeedWidget.h"
#include "Net/UnrealNetwork.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "CreatorMapSubsystem.h"
#include "CreatorStorageSubsystem.h"
#include "CreatorObject/SW_CreatorObject.h"

// Sets default values
ASW_Creator::ASW_Creator()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(RootSceneComponent);

	HandMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMeshComponent"));
	HandMeshComponent->SetupAttachment(RootSceneComponent);
	
	// Skeletal Mesh 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HandMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Ksw/Assets/MannequinsXR/Meshes/SKM_MannyXR_right.SKM_MannyXR_right'"));

	if (HandMesh.Succeeded())
	{
		HandMeshComponent->SetSkeletalMesh(HandMesh.Object);
	}

	// 카메라
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootSceneComponent);

	bReplicates = true; // Pawn이 네트워크에서 복제되도록 설정
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ASW_Creator::BeginPlay()
{
	Super::BeginPlay();
	
	// 위젯 생성
	CameraSpeedWidget = CreateWidget<USW_CameraSpeedWidget>(GetWorld(), CameraSpeedWidgetFactory);
	if (CameraSpeedWidget)
	{
		CameraSpeedWidget->AddToViewport();
	}

	if (IsLocallyControlled())
	{
		// 매쉬를 숨긴다.
		HandMeshComponent->SetVisibility(false);
	}
}

// Called every frame
void ASW_Creator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PC && MouseState != ECreatorMouseState::GizmoDrag)
	{
		PC->OnMouseOver();
	}
}

// Called to bind functionality to input
void ASW_Creator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PC  = Cast<ASW_CreatorPlayerController>(Controller);
	if (PC)
	{
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (subSys)
		{
			subSys->AddMappingContext(IMC_Player, 0);
		}
	}

	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// 움직임
	input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ASW_Creator::OnMyMove);
	input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ASW_Creator::OnMyLook);

	input->BindAction(IA_Q, ETriggerEvent::Triggered, this, &ASW_Creator::OnMyQ);
	input->BindAction(IA_W, ETriggerEvent::Triggered, this, &ASW_Creator::OnMyW);
	input->BindAction(IA_E, ETriggerEvent::Triggered, this, &ASW_Creator::OnMyE);
	input->BindAction(IA_R, ETriggerEvent::Triggered, this, &ASW_Creator::OnMyR);
	input->BindAction(IA_RClick, ETriggerEvent::Triggered, this, &ASW_Creator::OnMyRClick);
	input->BindAction(IA_LClick, ETriggerEvent::Triggered, this, &ASW_Creator::OnMyLClick);

	input->BindAction(IA_RClick, ETriggerEvent::Started, this, &ASW_Creator::OnMyRClickStarted);
	input->BindAction(IA_RClick, ETriggerEvent::Completed, this, &ASW_Creator::OnMyRClickCompleted);

	input->BindAction(IA_LClick, ETriggerEvent::Started, this, &ASW_Creator::OnMyLClickStarted);
	input->BindAction(IA_LClick, ETriggerEvent::Completed, this, &ASW_Creator::OnMyLClickCompleted);

	input->BindAction(IA_Del, ETriggerEvent::Triggered, this, &ASW_Creator::OnMyDelete);

	input->BindAction(IA_Ctrl, ETriggerEvent::Started, this, &ASW_Creator::OnMyCtrlStarted);
	input->BindAction(IA_Ctrl, ETriggerEvent::Completed, this, &ASW_Creator::OnMyCtrlCompleted);
	input->BindAction(IA_Alt, ETriggerEvent::Started, this, &ASW_Creator::OnMyAltStarted);
	input->BindAction(IA_Alt, ETriggerEvent::Completed, this, &ASW_Creator::OnMyAltCompleted);
	input->BindAction(IA_C, ETriggerEvent::Triggered, this, &ASW_Creator::OnMyC);
	input->BindAction(IA_V, ETriggerEvent::Triggered, this, &ASW_Creator::OnMyV);
	input->BindAction(IA_CameraSpeedUp, ETriggerEvent::Triggered, this, &ASW_Creator::OnMyCameraSpeedUp);
	input->BindAction(IA_CameraSpeedDown, ETriggerEvent::Triggered, this, &ASW_Creator::OnMyCameraSpeedDown);
}

void ASW_Creator::OnMyMove(const FInputActionValue& Value)
{
	if (MouseState == ECreatorMouseState::Clicked)
	{
		FVector2D v = Value.Get<FVector2D>();
		Direction.X = v.X;
		Direction.Y = v.Y;
		Direction.Normalize();
	}

	FTransform t = FTransform(GetControlRotation());
	Direction = t.TransformVector(Direction);

	FVector p = GetActorLocation();
	p += Direction * CameraSpeed * GetWorld()->DeltaTimeSeconds;
	SetActorLocation(p);

	Server_Movement(p);
	Direction = FVector::ZeroVector;
}

void ASW_Creator::OnMyLook(const FInputActionValue& Value)
{
	if (MouseState == ECreatorMouseState::Clicked)
	{
		FVector2D v = Value.Get<FVector2D>();

		AddControllerYawInput(v.X);
		AddControllerPitchInput(-v.Y);

		FRotator r = GetControlRotation();
		Server_Rotation(r);
	}
}

void ASW_Creator::OnMyQ(const FInputActionValue& Value)
{
	if (MouseState == ECreatorMouseState::Clicked)
	{
		Direction.Z = -1;
	}
	else
	{
		if (PC)
			PC->SetToolState(ECreatorToolState::Selection);
	}
	FTransform t = FTransform(GetControlRotation());
	Direction = t.TransformVector(Direction);

	FVector p = GetActorLocation();
	p += Direction * CameraSpeed * GetWorld()->DeltaTimeSeconds;
	SetActorLocation(p);

	Server_Movement(p);
	Direction = FVector::ZeroVector;
}

void ASW_Creator::OnMyW(const FInputActionValue& Value)
{

	if (MouseState == ECreatorMouseState::None)
	{
		if (PC)
			PC->SetToolState(ECreatorToolState::Position);
	}
}

void ASW_Creator::OnMyE(const FInputActionValue& Value)
{
	// 위로 이동
	if (MouseState == ECreatorMouseState::Clicked)
	{
		Direction.Z = 1;
	}
	else
	{
		if (PC)
			PC->SetToolState(ECreatorToolState::Rotation);
	}

	FTransform t = FTransform(GetControlRotation());
	Direction = t.TransformVector(Direction);

	FVector p = GetActorLocation();
	p += Direction * CameraSpeed * GetWorld()->DeltaTimeSeconds;
	SetActorLocation(p);

	Server_Movement(p);
	Direction = FVector::ZeroVector;
}

void ASW_Creator::OnMyR(const FInputActionValue& Value)
{
	if (MouseState == ECreatorMouseState::None)
	{
		if (PC)
			PC->SetToolState(ECreatorToolState::Scale);
	}
}

void ASW_Creator::OnMyRClick(const FInputActionValue& Value)
{
}

void ASW_Creator::OnMyLClick(const FInputActionValue& Value)
{
	if (MouseState == ECreatorMouseState::GizmoDrag)
		PC->Drag(MouseDownPosition);
}

void ASW_Creator::OnMyRClickStarted(const FInputActionValue& Value)
{
	SetMouseState(ECreatorMouseState::Clicked);
}

void ASW_Creator::OnMyRClickCompleted(const FInputActionValue& Value)
{
	SetMouseState(ECreatorMouseState::None);
}

void ASW_Creator::OnMyLClickStarted(const FInputActionValue& Value)
{
	SetMouseState(ECreatorMouseState::Clicked);

	if (PC)
	{
		if (PC->OnLeftClick())
		{
			SetMouseState(ECreatorMouseState::GizmoDrag);
		}

		PC->GetMousePosition(MouseDownPosition.X, MouseDownPosition.Y);
	}
}

void ASW_Creator::OnMyLClickCompleted(const FInputActionValue& Value)
{
	SetMouseState(ECreatorMouseState::None);

	if (PC)
	{
		PC->DragEnd();
	}
}

void ASW_Creator::OnMyDelete(const FInputActionValue& Value)
{
	if (PC)
	{
		PC->DeleteSelectedObject();
	}
}

void ASW_Creator::OnMyCtrlStarted(const FInputActionValue& Value)
{
	bIsCtrlPressed = true;
}

void ASW_Creator::OnMyCtrlCompleted(const FInputActionValue& Value)
{
	bIsCtrlPressed = false;
}

void ASW_Creator::OnMyAltStarted(const FInputActionValue& Value)
{
	bIsAltPressed = true;
}

void ASW_Creator::OnMyAltCompleted(const FInputActionValue& Value)
{
	bIsAltPressed = false;
}

void ASW_Creator::OnMyC(const FInputActionValue& Value)
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastInputTime < InputDelay)
	{
		return;
	}

	LastInputTime = CurrentTime;
	if (PC && bIsCtrlPressed)
	{
		PC->CopySelectedObject();
	}
}

void ASW_Creator::OnMyV(const FInputActionValue& Value)
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastInputTime < InputDelay)
	{
		return;
	}

	LastInputTime = CurrentTime;
	if (PC && bIsCtrlPressed)
	{
		PC->PasteSelectedObject();
	}
}

void ASW_Creator::OnMyCameraSpeedUp(const FInputActionValue& Value)
{
	if (10000.0f <= CameraSpeed) return;
	if (2000.0f <= CameraSpeed)
	{
		CameraSpeed += 500;
	}
	else
	{
		CameraSpeed += 100;
	}
	
	CameraSpeedWidget->SetCameraSpeed(CameraSpeed / 1000.0f);
}

void ASW_Creator::OnMyCameraSpeedDown(const FInputActionValue& Value)
{
	if (CameraSpeed <= 100) return;
	if (2000.0f < CameraSpeed)
	{
		CameraSpeed -= 500;
	}
	else
	{
		CameraSpeed -= 100;
	}

	CameraSpeedWidget->SetCameraSpeed(CameraSpeed / 1000.0f);
}

void ASW_Creator::SetMouseState(ECreatorMouseState NewState)
{
	if (MouseState == ECreatorMouseState::GizmoDrag && MouseState != NewState)
	{
		PC->SetInputMode(FInputModeGameAndUI());
		PC->SetShowMouseCursor(true);
	}

	MouseState = NewState;
	if (MouseState == ECreatorMouseState::GizmoDrag)
	{
		PC->SetInputMode(FInputModeGameAndUI());
		PC->SetShowMouseCursor(true);
	}
}

void ASW_Creator::Server_Movement_Implementation(const FVector& NewLocation)
{
	SetActorLocation(NewLocation);
}

void ASW_Creator::Server_Rotation_Implementation(const FRotator& NewRotation)
{
	SetActorRotation(NewRotation);
	CurrentRotation = NewRotation;

	if (HasAuthority())
	{
		OnRep_CurrentRotation();
	}
}

void ASW_Creator::OnRep_CurrentRotation()
{
	SetActorRotation(CurrentRotation);
}

void ASW_Creator::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASW_Creator, CurrentRotation);
}

// PC로 멀티케스트
void ASW_Creator::Multicast_AddCreatingDummyObject_Implementation(ASW_CreatorObject* NewCreatingObject, const struct FCreatorObjectData& ObjectData)
{
	UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
	NewCreatingObject->CreatingObjectData = new FCreatorObjectData(ObjectData);
	//NewCreatingObject->SetFileName(NewCreatingObject->CreatingObjectData->ItemName);
	system->AddObject(NewCreatingObject);

	// Creator에 붙어있는 PC를 가져옴
	if (PC)
	{
		PC->OnObjectChanged();
		PC->DoSelectObject(NewCreatingObject);
	}

	// 로컬 PC
	ASW_CreatorPlayerController* LocalPC = Cast<ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());

	if (LocalPC)
	{
		LocalPC->ReloadHierarchy();
	}
}

void ASW_Creator::Multicast_DeleteObjectInfo_Implementation(class ASW_CreatorObject* DeleteObject)
{
	if (PC)
	{
		PC->UnSelectServerDeleteObject(DeleteObject);
	}

	UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
	system->RemoveCreatorMapObject(DeleteObject);
}

void ASW_Creator::Multicast_DeleteObject_Implementation(ASW_CreatorObject* DeleteObject)
{
	// 로컬 PC
	ASW_CreatorPlayerController* LocalPC = Cast<ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());

	if (LocalPC)
	{
		LocalPC->ReloadHierarchy();
	}
}

void ASW_Creator::Multicast_DetachObject_Implementation(ASW_CreatorObject* DetachObject)
{
	if (!HasAuthority())
	{
		UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
		system->RemoveCreatorMapObject(DetachObject);
		system->AttachObject(nullptr, DetachObject);
	}

	// 로컬 PC
	ASW_CreatorPlayerController* LocalPC = Cast<ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());

	if (LocalPC)
	{
		LocalPC->ReloadHierarchy();
	}
}

void ASW_Creator::Multicast_AttachObject_Implementation(ASW_CreatorObject* ParentObject, ASW_CreatorObject* AttachObject)
{
	if (!HasAuthority())
	{
		UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
		if (system->IsChildObject(AttachObject, ParentObject))
		{
			return;
		}

		system->RemoveCreatorMapObject(AttachObject);
		system->AttachObject(ParentObject, AttachObject);
	}

	// 로컬 PC
	ASW_CreatorPlayerController* LocalPC = Cast<ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());

	if (LocalPC)
	{
		LocalPC->ReloadHierarchy();
	}
}

void ASW_Creator::Multicast_CopyPasteObject_Implementation(ASW_CreatorObject* CopyObject)
{
	if (PC)
	{
		PC->DoSelectObject(CopyObject);
	}

	if (!HasAuthority())
	{
		UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
		ASW_CreatorObject* PrevParent = system->FindParentObject(CopyObject);

		if (PrevParent == nullptr)
		{
			system->AddCreatorMapObject(CopyObject);
		}
	}

	// 로컬 PC
	ASW_CreatorPlayerController* LocalPC = Cast<ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());
	if (LocalPC)
	{
		LocalPC->ReloadHierarchy();
	}
}
