// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_Player.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/StaticMesh.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "HG_PlayerGoodsComponent.h"
#include "HG_StoreTriggerBox.h"
#include "HG_ItemBase.h"
#include "HG_PlayerInventoryComponent.h"
#include "InventoryWidget.h"

// Sets default values
AHG_Player::AHG_Player()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 200.0f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);

	GoodsComp = CreateDefaultSubobject<UHG_PlayerGoodsComponent>(TEXT("GoodsComp"));

	InventoryComp = CreateDefaultSubobject<UHG_PlayerInventoryComponent>(TEXT("InventoryComp"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -80.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));
}

// Called when the game starts or when spawned
void AHG_Player::BeginPlay()
{
	Super::BeginPlay();

	auto* pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSys)
		{
			subSys->AddMappingContext(IMC_Player, 0);
		}
	}
	GoodsComp->SetGold(0);
}

// Called every frame
void AHG_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	FTransform t = FTransform(GetControlRotation());
	Direction = t.TransformVector(Direction);

	AddMovementInput(Direction);
	Direction = FVector::ZeroVector;

}

// Called to bind functionality to input
void AHG_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// 움직임
	input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AHG_Player::OnMyMove);
	input->BindAction(IA_Jump, ETriggerEvent::Started, this, &AHG_Player::OnMyJump);
	input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AHG_Player::OnMyLook);

	// 상호작용
	input->BindAction(IA_Interaction, ETriggerEvent::Completed, this, &AHG_Player::OnMyInteraction);

	// 인벤토리
	input->BindAction(IA_Inventory, ETriggerEvent::Completed, this, &AHG_Player::PopUpInventory);

}

void AHG_Player::OnMyMove(const FInputActionValue& Value)
{
	if (!bCanMove) return;

	FVector2D v = Value.Get<FVector2D>();
	Direction.X = v.X;
	Direction.Y = v.Y;
	Direction.Normalize();
}

void AHG_Player::OnMyJump(const FInputActionValue& Value)
{
	if (!bCanMove) return;
	Jump();
}

void AHG_Player::OnMyLook(const FInputActionValue& Value)
{
	if (!bCanMove) return;

	FVector2D v = Value.Get<FVector2D>();
	AddControllerPitchInput(-v.Y);
	AddControllerYawInput(v.X);
}

void AHG_Player::OnMyInteraction(const FInputActionValue& Value)
{
	DetectObject();
}

void AHG_Player::DetectObject()
{
	FHitResult OutHit;
	FVector Start = GetActorLocation();
	FVector End = Start + CameraComp->GetForwardVector() * 1000.0f;

	ECollisionChannel TC = ECC_WorldDynamic;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, TC, Params);
	if (bHit)
	{
		DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Yellow, false, 1.0f);
		// 라인트레이스에 맞은게 상점 트리거 박스일 때
		if (auto* STB = Cast<AHG_StoreTriggerBox>(OutHit.GetActor()))
		{
			STB->ByInteraction();
			auto* pc = Cast<APlayerController>(Controller);
			if (pc)
			{
				if (!bToggle)
				{
					pc->SetShowMouseCursor(true);
					bToggle = !bToggle;
					bCanMove = false;
				}
				else
				{
					pc->SetShowMouseCursor(false);
					bToggle = !bToggle;
					bCanMove = true;
				}
			}
		}
		// 라인트레이스에 맞은게 아이템일 때
		if (auto* Item = Cast<AHG_ItemBase>(OutHit.GetActor()))
		{
			InventoryComp->AddtoInventory(Item->GetItemData(), 1);
			Item->Destroy();
		}
	}
}

void AHG_Player::PopUpInventory(const FInputActionValue& Value)
{
	if (InventoryWidget == nullptr)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
		if (InventoryWidget)
		{
			InventoryWidget->SetOwner(this);
		}
	}
	auto* pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		if (!bToggle)
		{
			pc->SetShowMouseCursor(true);
			InventoryWidget->AddToViewport();
			InventoryWidget->InitInventoryUI();
			bToggle = !bToggle;
			bCanMove = false;
		}
		else
		{
			pc->SetShowMouseCursor(false);
			InventoryWidget->RemoveFromParent();
			bToggle = !bToggle;
			bCanMove = true;
		}

	}
}

