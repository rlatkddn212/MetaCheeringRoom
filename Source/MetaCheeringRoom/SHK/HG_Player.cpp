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
#include "HG_DisplayStandBase.h"
#include "HG_ItemPurchaseWidget.h"
#include "HG_EquipItem.h"
#include "GameFramework/PlayerController.h"

AHG_Player::AHG_Player()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 200.0f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);

	GoodsComp = CreateDefaultSubobject<UHG_PlayerGoodsComponent>(TEXT("GoodsComp"));

	InventoryComp = CreateDefaultSubobject<UHG_PlayerInventoryComponent>(TEXT("InventoryComp"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -80.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));

	HandComp = CreateDefaultSubobject<USceneComponent>(TEXT("HandComp"));
	HandComp->SetupAttachment(GetMesh(), TEXT("HandPosition"));

	LowerComp = CreateDefaultSubobject<USceneComponent>(TEXT("LowerComp"));
	LowerComp->SetupAttachment(GetMesh(), TEXT("LowerPosition"));

	UpperComp = CreateDefaultSubobject<USceneComponent>(TEXT("UpperComp"));
	UpperComp->SetupAttachment(GetMesh(), TEXT("UpperPosition"));

	bReplicates = true;
	SetReplicateMovement(true);
}

void AHG_Player::BeginPlay()
{
	Super::BeginPlay();
	DetectedStand = nullptr;

	auto* pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSys)
		{
			subSys->AddMappingContext(IMC_Player, 0);
		}
	}
	GoodsComp->SetGold(4000);
}

void AHG_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FTransform t = FTransform(GetControlRotation());
	Direction = t.TransformVector(Direction);

	AddMovementInput(Direction);
	Direction = FVector::ZeroVector;

	FHitResult OutHit;
	FVector Start = GetActorLocation();
	FVector End = Start + CameraComp->GetForwardVector() * 300.0f;

	ECollisionChannel TC = ECC_WorldDynamic;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, TC, Params);
	if (bHit)
	{
		DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Green, false, 1.0f);
		LookingPoint = OutHit.ImpactPoint;

		if (LookAtActor != OutHit.GetActor() && bIsStand)
		{
			bIsStand = false;
			DetectedStand->Detected(false, this);
			DetectedStand = nullptr;
		}
		else
		{
			if (!bIsStand)
			{
				DetectedStand = Cast<AHG_DisplayStandBase>(OutHit.GetActor());
				if (DetectedStand != nullptr)
				{
					DetectedStand->Detected(true, this);
					bIsStand = true;
				}
			}
		}
		LookAtActor = OutHit.GetActor();
	}
	else
	{
		if (DetectedStand)
		{
			bIsStand = false;
			DetectedStand->Detected(false, this);
			DetectedStand = nullptr;
		}
	} 

	if (bDetectStand)
	{
		SpringArmComp->TargetArmLength = FMath::FInterpTo(SpringArmComp->TargetArmLength, TargetValue2, DeltaTime, 5.0f);
	}
	else                 
	{
		SpringArmComp->TargetArmLength = FMath::FInterpTo(SpringArmComp->TargetArmLength, TargetValue1, DeltaTime, 5.0f);
	}
}

void AHG_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AHG_Player::OnMyMove);
	input->BindAction(IA_Jump, ETriggerEvent::Started, this, &AHG_Player::OnMyJump);
	input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AHG_Player::OnMyLook);

	input->BindAction(IA_Interaction, ETriggerEvent::Completed, this, &AHG_Player::OnMyInteraction);

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
		else if (auto* Item = Cast<AHG_ItemBase>(OutHit.GetActor()))
		{
			InventoryComp->AddtoInventory(Item->GetItemData(), 1);
			Item->Destroy();
		}
		else if (auto* Stand = Cast<AHG_DisplayStandBase>(OutHit.GetActor()))
		{
			TempData = Stand->ItemData;
			PopUpPurchaseWidget();
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
			InventoryWidget->AddToViewport();
			InventoryWidget->InitInventoryUI();
			pc->SetShowMouseCursor(true);
			bToggle = !bToggle;
			bCanMove = false;
		}
		else
		{
			InventoryWidget->RemoveFromParent();
			pc->SetShowMouseCursor(false);
			bToggle = !bToggle;
			bCanMove = true;
		}
	}
}

void AHG_Player::PopUpPurchaseWidget()
{
	if (PurchaseWidget == nullptr)
	{
		PurchaseWidget = CreateWidget<UHG_ItemPurchaseWidget>(GetWorld(), PurchaseWidgetClass);
		PurchaseWidget->SetOwner(this);
		PurchaseWidget->SetItemInfo(TempData);
	}
	auto* pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		if (!bToggle)
		{
			PurchaseWidget->AddToViewport();
			bCanMove = false;
			pc->SetShowMouseCursor(true);
			bToggle = !bToggle;
		}
		else
		{
			PurchaseWidget->RemoveFromParent();
			bCanMove = true;
			pc->SetShowMouseCursor(false);
			bToggle = !bToggle;
		}
	}
}

void AHG_Player::EquipItem(AHG_EquipItem* ItemValue)
{
	GrabItem = ItemValue;
	EquipItemList.Add(GrabItem);
	auto* mesh = GrabItem->GetComponentByClass<UStaticMeshComponent>();
	check(mesh)
		if (mesh)
		{
			mesh->SetSimulatePhysics(false);
			switch (GrabItem->GetItemCategory())
			{
			case EItemCategory::Category_Bottom:
				mesh->AttachToComponent(LowerComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
			case EItemCategory::Category_Top:
				mesh->AttachToComponent(UpperComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
			case EItemCategory::Category_HandGrab:
				mesh->AttachToComponent(HandComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
			default:
				break;
			}
		}
}

void AHG_Player::UnequipItem(const FString& NameValue)
{
	for (AHG_EquipItem* item : EquipItemList)
	{
		if (item->GetItemName() == NameValue)
		{
			auto* mesh = item->GetComponentByClass<UStaticMeshComponent>();
			check(mesh);
			if (mesh)
			{
				mesh->SetSimulatePhysics(true);
				mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
			}
			item->Destroy();
			EquipItemList.Remove(item);
		}
	}
}

void AHG_Player::EquipItemToSocket(AHG_EquipItem* ItemValue)
{
	ServerRPCEquipItemToSocket(ItemValue);
}

void AHG_Player::UnequipItemToSocket(const FString& NameValue)
{
	ServerRPCUnequipItemToSocket(NameValue);
}

void AHG_Player::ServerRPCEquipItemToSocket_Implementation(AHG_EquipItem* ItemValue)
{
	MulticastRPCEquipItemToSocket(ItemValue);
}

void AHG_Player::MulticastRPCEquipItemToSocket_Implementation(AHG_EquipItem* ItemValue)
{
	EquipItem(ItemValue);
}

void AHG_Player::ServerRPCUnequipItemToSocket_Implementation(const FString& NameValue)
{
	MulticastRPCUnequipItemToSocket(NameValue);
}

void AHG_Player::MulticastRPCUnequipItemToSocket_Implementation(const FString& NameValue)
{
	UnequipItem(NameValue);
}

