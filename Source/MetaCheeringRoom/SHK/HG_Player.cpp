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
#include "Net/UnrealNetwork.h"
#include "HG_GameInstance.h"

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
	HandComp->SetIsReplicated(true);

	LowerComp = CreateDefaultSubobject<USceneComponent>(TEXT("LowerComp"));
	LowerComp->SetupAttachment(GetMesh(), TEXT("LowerPosition"));
	LowerComp->SetIsReplicated(true);

	UpperComp = CreateDefaultSubobject<USceneComponent>(TEXT("UpperComp"));
	UpperComp->SetupAttachment(GetMesh(), TEXT("UpperPosition"));
	UpperComp->SetIsReplicated(true);

	bReplicates = true;
	SetReplicateMovement(true);
}

void AHG_Player::BeginPlay()
{
	Super::BeginPlay();
	DetectedStand = nullptr;
	
	GI = Cast<UHG_GameInstance>(GetWorld()->GetGameInstance());

	auto* pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSys)
		{
			subSys->AddMappingContext(IMC_Player, 0);
		}	
		pc->SetShowMouseCursor(false);
		pc->SetInputMode(FInputModeGameOnly());
	}
	InitEquipItemList = GI->EquipItemInfoList;
	for (auto ID : InitEquipItemList)
	{
		EquipItemToSocket(ID);
	}

	InventoryComp->Inventory = GI->CurrentInventory;
	GoodsComp->SetGold(GI->CurrentGold);
}

void AHG_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FTransform t = FTransform(GetControlRotation());
	Direction = t.TransformVector(Direction);

	AddMovementInput(Direction);
	Direction = FVector::ZeroVector;

	FHitResult OutHit;
	int32 BoneIndex = GetMesh()->GetBoneIndex(TEXT("head"));
	FVector Start = this->GetMesh()->GetBoneTransform(BoneIndex).GetLocation();
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
		SpringArmComp->TargetArmLength = FMath::FInterpTo(SpringArmComp->TargetArmLength, TargetValue2, DeltaTime, 3.5f);
	}
	else
	{
		SpringArmComp->TargetArmLength = FMath::FInterpTo(SpringArmComp->TargetArmLength, TargetValue1, DeltaTime, 2.0f);
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
	int32 BoneIndex = GetMesh()->GetBoneIndex(TEXT("head"));
	FVector Start = this->GetMesh()->GetBoneTransform(BoneIndex).GetLocation();
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
			DestroyItem(Item);
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
	}
	auto* pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		PurchaseWidget->SetItemInfo(TempData);
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
	if (ItemValue == nullptr) return;
	EquipItemList.Add(ItemValue);
	GI->EquipItemInfoList.Add(ItemValue->GetItemData());
	auto* mesh = ItemValue->GetComponentByClass<UStaticMeshComponent>();
	check(mesh);
	if (mesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attach"));
		mesh->SetSimulatePhysics(false);
		mesh->AttachToComponent(HandComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
		switch (ItemValue->GetItemCategory())
		{
		case EItemCategory::Category_Bottom:
			mesh->AttachToComponent(LowerComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
			break;
		case EItemCategory::Category_Top:
			mesh->AttachToComponent(UpperComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
			break;
		case EItemCategory::Category_HandGrab:
			mesh->AttachToComponent(HandComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
			break;
		default:
			break;
		}
	}
}


void AHG_Player::UnequipItem(const FString& NameValue)
{
	TArray<AHG_EquipItem*> ItemsToRemove;

	for (AHG_EquipItem* item : EquipItemList)
	{
		if (item->GetItemName() == NameValue)
		{
			ItemsToRemove.Add(item);
		}
	}
	for (auto* item : ItemsToRemove)
	{
		if (item != nullptr && item->GetItemName() == NameValue)
		{
			UStaticMeshComponent* mesh = item->GetComponentByClass<UStaticMeshComponent>();
			check(mesh);
			if (mesh)
			{
				mesh->SetSimulatePhysics(true);
				mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
			}
			GI->EquipItemInfoList.Remove(item->GetItemData());
			EquipItemList.Remove(item);
			item->Destroy();
		}
	}
}

void AHG_Player::EquipItemToSocket(FItemData p_ItemInfo)
{
	ServerRPCEquipItemToSocket(p_ItemInfo);
}

void AHG_Player::UnequipItemToSocket(const FString& NameValue)
{
	ServerRPCUnequipItemToSocket(NameValue);
}


void AHG_Player::DestroyItem(AHG_ItemBase* ItemValue)
{
	ServerRPCDestroyItem(ItemValue);
}


void AHG_Player::SpawnItem(FItemData p_ItemInfo)
{
	ServerRPCSpawnItem(p_ItemInfo);

}

void AHG_Player::ServerRPCSpawnItem_Implementation(FItemData p_ItemInfo)
{
	auto* SpawnedItem = GetWorld()->SpawnActor<AHG_ItemBase>(p_ItemInfo.ItemClass, GetActorLocation(), GetActorRotation());
	if (SpawnedItem != nullptr)
	{
		//MulticastRPCSpawnItem(SpawnedItem);
		SpawnedItem->SetReplicates(true);
		SpawnedItem->SetOwner(this);
		SpawnedItem->SetActorHiddenInGame(true);
		SpawnedItem->Use();
	}
}

void AHG_Player::MulticastRPCSpawnItem_Implementation(AHG_ItemBase* ItemValue)
{
	if (ItemValue)
	{
		ItemValue->SetReplicates(true);
		ItemValue->SetOwner(this);
		ItemValue->SetActorHiddenInGame(true);
		ItemValue->Use();
	}
}

void AHG_Player::ServerRPCDestroyItem_Implementation(AHG_ItemBase* ItemValue)
{
	ItemValue->Destroy();
}


void AHG_Player::ServerRPCEquipItemToSocket_Implementation(FItemData p_ItemInfo)
{
	auto* EItem = GetWorld()->SpawnActor<AHG_EquipItem>(p_ItemInfo.ItemClass, GetActorLocation(), GetActorRotation());
	if (EItem)
	{
		EItem->SetOwner(this);

		MulticastRPCEquipItemToSocket(EItem);

	}
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

void AHG_Player::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHG_Player, EquipItemList);
}


