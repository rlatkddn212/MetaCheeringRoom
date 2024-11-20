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
#include "HG_PlayerAnimInstance.h"
#include "HG_StoreWidget.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HG_RemoteCS.h"
#include "HG_HUD.h"
#include "HG_CheeringStick.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "HG_CustomUI.h"

AHG_Player::AHG_Player()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);

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

	HandLComp = CreateDefaultSubobject<USceneComponent>(TEXT("HandLComp"));
	HandLComp->SetupAttachment(GetMesh(), TEXT("HandLPosition"));
	HandLComp->SetIsReplicated(true);

	HandRComp = CreateDefaultSubobject<USceneComponent>(TEXT("HandRComp"));
	HandRComp->SetupAttachment(GetMesh(), TEXT("HandRPosition"));
	HandRComp->SetIsReplicated(true);

	LowerComp = CreateDefaultSubobject<USceneComponent>(TEXT("LowerComp"));
	LowerComp->SetupAttachment(GetMesh(), TEXT("LowerPosition"));
	LowerComp->SetIsReplicated(true);

	UpperComp = CreateDefaultSubobject<USceneComponent>(TEXT("UpperComp"));
	UpperComp->SetupAttachment(GetMesh(), TEXT("UpperPosition"));
	UpperComp->SetIsReplicated(true);

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetIsReplicated(true);

	bReplicates = true;
	SetReplicateMovement(true);
	SetReplicates(true);
}

void AHG_Player::BeginPlay()
{
	Super::BeginPlay();

	Anim = Cast<UHG_PlayerAnimInstance>(GetMesh()->GetAnimInstance());

	DetectedStand = nullptr;

	GI = Cast<UHG_GameInstance>(GetWorld()->GetGameInstance());

	PC = Cast<APlayerController>(Controller);

	if (PC)
	{
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (subSys)
		{
			subSys->AddMappingContext(IMC_Player, 0);
		}

		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
	}



	TargetValue1 = SpringArmComp->TargetArmLength;

	// 커스텀 - 옷
	UMaterialInterface* Mat_Cloth = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/LYJ/Characters/Mint/Materials/Custom/M_CUSTOM_Mint_Cloth.M_CUSTOM_Mint_Cloth'"));
	if (Mat_Cloth)
	{
		DynamicMaterial_Cloth = UMaterialInstanceDynamic::Create(Mat_Cloth, this);
		if (DynamicMaterial_Cloth)
		{
			GetMesh()->SetMaterial(2, DynamicMaterial_Cloth);
		}
	}
	// 커스텀 - 옷색깔
	UMaterialInterface* Mat_ClothColor = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/LYJ/Characters/Mint/Materials/Custom/M_CUSTOM_Mint_Cloth_PointColor.M_CUSTOM_Mint_Cloth_PointColor'"));
	if (Mat_ClothColor)
	{
		DynamicMaterial_ClothColor = UMaterialInstanceDynamic::Create(Mat_ClothColor, this);
		if (DynamicMaterial_ClothColor)
		{
			GetMesh()->SetMaterial(1, DynamicMaterial_ClothColor);
		}
	}
	// 커스텀 - 머리
	UMaterialInterface* Mat_Hair = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/LYJ/Characters/Mint/Materials/Custom/M_CUSTOM_Mint_Hair.M_CUSTOM_Mint_Hair'"));
	if (Mat_Hair)
	{
		DynamicMaterial_Hair = UMaterialInstanceDynamic::Create(Mat_Hair, this);
		if (DynamicMaterial_Hair)
		{
			GetMesh()->SetMaterial(8, DynamicMaterial_Hair);
		}
	}
	// 커스텀 - 눈
	UMaterialInterface* Mat_Eyes = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/LYJ/Characters/Mint/Materials/Custom/M_CUSTOM_Mint_Eye.M_CUSTOM_Mint_Eye'"));
	if (Mat_ClothColor)
	{
		DynamicMaterial_Eyes = UMaterialInstanceDynamic::Create(Mat_Eyes, this);
		if (DynamicMaterial_Eyes)
		{
			GetMesh()->SetMaterial(6, DynamicMaterial_Eyes);
		}
	}
	// 커스텀 - 머리핀
	UMaterialInterface* Mat_HairPin = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/LYJ/Characters/Mint/Materials/Custom/M_CUSTOM_Mint_Cloth_PointColor.M_CUSTOM_Mint_Cloth_PointColor'"));
	if (Mat_ClothColor)
	{
		DynamicMaterial_HairPin = UMaterialInstanceDynamic::Create(Mat_HairPin, this);
		if (DynamicMaterial_HairPin)
		{
			GetMesh()->SetMaterial(7, DynamicMaterial_HairPin);
		}
	}

	if (GI)
	{
		if (IsLocallyControlled())
		{
			InitEquipItemList = GI->EquipItemInfoList;
			for (auto ID : InitEquipItemList)
			{
				EquipItemToSocket(ID);
			}
			InventoryComp->Inventory = GI->CurrentInventory;

			GoodsComp->SetGold(GI->CurrentGold);
		}
		if (HasAuthority())
		{
			CurCloth = GI->CurCloth;
			CurPrinting = GI->CurPrinting;
			CurClothHem = GI->CurClothHem;
			CurEyes = GI->CurEyes;
			CurHair = GI->CurHair;
			CurHairPin = GI->CurHairPin;
		}
		if (IsLocallyControlled())
		{
			ServerRPC_ApplyCustom(GI->CurCloth, GI->CurPrinting, GI->CurClothHem, GI->CurEyes, GI->CurHair, GI->CurHairPin);
		}
	}
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
	FVector End = Start + CameraComp->GetForwardVector() * 200.0f;

	ECollisionChannel TC = ECC_WorldDynamic;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, TC, Params);
	if (bHit)
	{
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

	Anim = Cast<UHG_PlayerAnimInstance>(GetMesh()->GetAnimInstance());

	if (bDetectStand)
	{
		Timing = 1.0f;
		SpringArmComp->TargetArmLength = FMath::FInterpTo(SpringArmComp->TargetArmLength, TargetValue2, DeltaTime, 3.5f);
	}
	else
	{
		Timing -= DeltaTime;
		if (Timing <= 0)
		{
			SpringArmComp->TargetArmLength = FMath::FInterpTo(SpringArmComp->TargetArmLength, TargetValue1, DeltaTime, 2.0f);
		}
	}
	// 상점 UI 업데이트
	if (StoreWidget)
	{
		StoreWidget->SetPointText(this->GoodsComp->GetGold());
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

	input->BindAction(IA_Emotion, ETriggerEvent::Completed, this, &AHG_Player::Emotion);

	input->BindAction(IA_Custom, ETriggerEvent::Completed, this, &AHG_Player::PopUpCustomUI);
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
		if (auto* Item = Cast<AHG_ItemBase>(OutHit.GetActor()))
		{
			if (!Item->bEquiped)
			{
				InventoryComp->AddtoInventory(Item->GetItemData(), 1);
				DestroyItem(Item);
			}
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
	if (PC)
	{
		if (!bToggle)
		{
			InventoryWidget->AddToViewport();
			InventoryWidget->InitInventoryUI();
			PC->SetShowMouseCursor(true);
			bToggle = !bToggle;
			bCanMove = false;
		}
		else
		{
			InventoryWidget->RemoveFromParent();
			PC->SetShowMouseCursor(false);
			bToggle = !bToggle;
			bCanMove = true;
		}
	}
}

void AHG_Player::Emotion()
{
	if (bEquipItem)
	{
		auto* RCSWidget = CreateWidget<UHG_RemoteCS>(GetWorld(), RCSClass);
		if (RCSWidget)
		{
			RCSWidget->AddToViewport();
			RCSWidget->SetOwner(this);
			if (PC)
			{
				PC->SetShowMouseCursor(true);
				FInputModeUIOnly InputMode;
				PC->SetInputMode(InputMode);
			}
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
	if (PC)
	{
		PurchaseWidget->SetItemInfo(TempData);
		if (!bToggle)
		{
			bCanMove = false;
			PC->SetShowMouseCursor(true);
			PC->SetInputMode(FInputModeGameAndUI());
			PurchaseWidget->AddToViewport();
			bToggle = !bToggle;
		}
		// 		else
		// 		{
		// 			PurchaseWidget->RemoveFromParent();
		// 			bCanMove = true;
		// 			PC->SetShowMouseCursor(false);
		// 			PC->SetInputMode(FInputModeGameOnly());
		// 			bToggle = !bToggle;
		// 		}
	}
}

void AHG_Player::EquipItem(AHG_EquipItem* ItemValue)
{
	if (ItemValue == nullptr) return;
	ItemValue->bEquiped = true;

	if (IsLocallyControlled())
	{
		EquipItemList.Add(ItemValue);
		GI->EquipItemInfoList.Add(ItemValue->GetItemData());
	}

	auto* mesh = ItemValue->GetComponentByClass<UStaticMeshComponent>();
	check(mesh);
	if (mesh)
	{
		bEquipItem = true;

		mesh->SetSimulatePhysics(false);

		switch (ItemValue->GetItemCategory())
		{
		case EItemCategory::Category_Bottom:
			mesh->AttachToComponent(LowerComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
			break;
		case EItemCategory::Category_Top:
			mesh->AttachToComponent(UpperComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
			break;
		case EItemCategory::Category_OneHandGrab:
			mesh->AttachToComponent(HandRComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
			if (HUD)
			{
				HUD->UpdateHUD(TEXT("E) 응원봉 조작하기"));
			}
			break;
		case EItemCategory::Category_TwoHandGrab:
			if (!bPassed)
			{
				mesh->AttachToComponent(HandRComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
				bPassed = true;
				EquipItemToSocket(ItemValue->GetItemData());
			}
			else
			{
				mesh->AttachToComponent(HandLComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
				if (HUD)
				{
					HUD->UpdateHUD(TEXT("E) 응원봉 조작하기"));
				}
				bPassed = false;
			}
			break;
		default:
			break;
		}
	}
}


void AHG_Player::UnequipItem(const FString& NameValue)
{
	TArray<AHG_EquipItem*> ItemsToRemove;

	bEquipItem = false;

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

void AHG_Player::SpawnedMulticast()
{
	MulticastRPCEquipItemToSocket(EItem);
}


void AHG_Player::SpawnItem(FItemData p_ItemInfo)
{
	ServerRPCSpawnItem(p_ItemInfo);
}

void AHG_Player::CreateStoreWidget()
{
	if (StoreWidgetFactory)
	{
		StoreWidget = CreateWidget<UHG_StoreWidget>(GetWorld(), StoreWidgetFactory);
		if (StoreWidget)
		{
			StoreWidget->AddToViewport();
			bStoreWidget = true;
		}
	}
}

void AHG_Player::EnterTheStore()
{
	if (StoreWidget == nullptr)
	{
		CreateStoreWidget();
	}
	else
	{
		if (!bStoreWidget)
		{
			StoreWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			StoreWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	bStoreWidget = !bStoreWidget;
}

void AHG_Player::ExitTheStore()
{
	if (StoreWidget != nullptr)
	{
		StoreWidget->RemoveFromParent();
		StoreWidget = nullptr;
	}
}

void AHG_Player::PopUpHUD()
{
	if (IsLocallyControlled())
	{
		if (HeadUpClass)
		{
			HUD = CreateWidget<UHG_HUD>(GetWorld(), HeadUpClass);
			if (HUD)
			{
				if (PC)
				{
					HUD->SetOwningPlayer(PC);
				}
				HUD->AddToViewport();
			}
		}
	}
}

void AHG_Player::ApplyCustomizing(FLinearColor Cloth, UTexture2D* ClothTexture, FLinearColor ClothHem, FLinearColor Eyes, FLinearColor Hair, FLinearColor HairPin)
{
	GI->CurCloth = Cloth;
	GI->CurClothHem = ClothHem;
	if (ClothTexture) GI->CurPrinting = ClothTexture;
	GI->CurHair = Hair;
	GI->CurHairPin = HairPin;
	GI->CurEyes = Eyes;

	if (DynamicMaterial_Cloth)
	{
		DynamicMaterial_Cloth->SetVectorParameterValue("CustomColor", Cloth);

		if (ClothTexture)
		{
			DynamicMaterial_Cloth->SetScalarParameterValue("UseCustomTexture", 1.0f);
			DynamicMaterial_Cloth->SetTextureParameterValue("CustomTexture", ClothTexture);
		}
		else
		{
			DynamicMaterial_Cloth->SetScalarParameterValue("UseCustomTexture", 0.0f);
		}
	}
	if (DynamicMaterial_ClothColor)
	{
		DynamicMaterial_ClothColor->SetVectorParameterValue("CustomColor", ClothHem);
	}
	if (DynamicMaterial_Eyes)
	{
		if (!(Eyes == FLinearColor(1.0f, 1.0f, 1.0f))) DynamicMaterial_Eyes->SetVectorParameterValue("CustomColor", Eyes);
	}
	if (DynamicMaterial_Hair)
	{
		DynamicMaterial_Hair->SetVectorParameterValue("CustomColor", Hair);
	}
	if (DynamicMaterial_HairPin)
	{
		DynamicMaterial_HairPin->SetVectorParameterValue("CustomColor", HairPin);
	}
}

void AHG_Player::PopUpCustomUI()
{
	if (CustomUIClass)
	{
		auto* CustomUI = CreateWidget<UHG_CustomUI>(GetWorld(), CustomUIClass);
		if (CustomUI)
		{
			CustomUI->AddToViewport();
			CustomUI->SetOwningPlayer(PC);
			if (PC)
			{
				PC->SetShowMouseCursor(true);
				PC->SetInputMode(FInputModeUIOnly());
			}
		}
	}
}

void AHG_Player::UpdateClothColor()
{
	if (DynamicMaterial_Cloth)
	{
		DynamicMaterial_Cloth->SetVectorParameterValue("CustomColor", CurCloth);
	}
}

void AHG_Player::UpdateHairColor()
{
	if (DynamicMaterial_Hair)
	{
		DynamicMaterial_Hair->SetVectorParameterValue("CustomColor", CurHair);
	}
}

void AHG_Player::UpdateHairPinColor()
{
	if (DynamicMaterial_HairPin)
	{
		DynamicMaterial_HairPin->SetVectorParameterValue("CustomColor", CurHairPin);
	}
}

void AHG_Player::UpdateEyesColor()
{
	if (DynamicMaterial_Eyes)
	{
		DynamicMaterial_Eyes->SetVectorParameterValue("CustomColor", CurEyes);
	}
}

void AHG_Player::UpdateClothHemColor()
{
	if (DynamicMaterial_ClothColor)
	{
		DynamicMaterial_ClothColor->SetVectorParameterValue("CustomColor", CurClothHem);
	}
}

void AHG_Player::UpdateClothPrinting()
{
	if (DynamicMaterial_Cloth)
	{
		if (CurPrinting)
		{
			DynamicMaterial_Cloth->SetScalarParameterValue("UseCustomTexture", 1.0f);
			DynamicMaterial_Cloth->SetTextureParameterValue("CustomTexture", CurPrinting);
		}
		else
		{
			DynamicMaterial_Cloth->SetScalarParameterValue("UseCustomTexture", 0.0f);
		}
	}
}

void AHG_Player::OnRep_ClothColor()
{
	UpdateClothColor();
}

void AHG_Player::OnRep_ClothHemColor()
{
	UpdateClothHemColor();
}

void AHG_Player::OnRep_HairPinColor()
{
	UpdateHairPinColor();
}

void AHG_Player::OnRep_HairColor()
{
	UpdateHairColor();
}

void AHG_Player::OnRep_EyesColor()
{
	UpdateEyesColor();
}

void AHG_Player::OnRep_Printing()
{
	UpdateClothPrinting();
}

void AHG_Player::Multicast_ApplyCustom_Implementation(FLinearColor Cloth, UTexture2D* ClothTexture, FLinearColor ClothHem, FLinearColor Eyes, FLinearColor Hair, FLinearColor HairPin)
{
	ApplyCustomizing(Cloth, ClothTexture, ClothHem, Eyes, Hair, HairPin);
}

void AHG_Player::ServerRPC_ApplyCustom_Implementation(FLinearColor Cloth, UTexture2D* ClothTexture, FLinearColor ClothHem, FLinearColor Eyes, FLinearColor Hair, FLinearColor HairPin)
{
	Multicast_ApplyCustom(Cloth, ClothTexture, ClothHem, Eyes, Hair, HairPin);
}

void AHG_Player::ServerRPC_Emotion_Implementation(UAnimMontage* Montage)
{
	MulticastRPC_Emotion(Montage);
}

void AHG_Player::MulticastRPC_Emotion_Implementation(UAnimMontage* Montage)
{
	Anim->PlaySelectedMontage(Montage);
}

void AHG_Player::ServerRPCSpawnItem_Implementation(FItemData p_ItemInfo)
{
	FVector SpawnLocation;
	FRotator SpawnRotation;
	if (p_ItemInfo.ItemCategory == EItemCategory::Category_Emoji)
	{
		SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, 150.0f);
		SpawnRotation = GetActorRotation() + FRotator(90.0f, 0.0f, 0.0f);
	}
	else
	{
		SpawnLocation = GetActorLocation();
		SpawnRotation = GetActorRotation();
	}
	auto* SpawnedItem = GetWorld()->SpawnActor<AHG_ItemBase>(p_ItemInfo.ItemClass, SpawnLocation, SpawnRotation);

	if (SpawnedItem != nullptr)
	{
		SpawnedItem->SetReplicates(true);
		SpawnedItem->SetItemOwner(this);
		if (SpawnedItem->GetItemData().ItemCategory != EItemCategory::Category_Emoji)
		{
			SpawnedItem->SetActorHiddenInGame(true);
		}
		MulticastRPCSpawnItem(SpawnedItem);
	}
}

void AHG_Player::MulticastRPCSpawnItem_Implementation(AHG_ItemBase* ItemValue)
{
	if (ItemValue)
	{
		ItemValue->Use();
	}
}

void AHG_Player::ServerRPCDestroyItem_Implementation(AHG_ItemBase* ItemValue)
{
	ItemValue->Destroy();
}


void AHG_Player::ServerRPCEquipItemToSocket_Implementation(FItemData p_ItemInfo)
{
	EItem = GetWorld()->SpawnActor<AHG_EquipItem>(p_ItemInfo.ItemClass, GetActorLocation(), GetActorRotation());

	if (EItem)
	{
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, this, &AHG_Player::SpawnedMulticast, 0.1f, false);
	}
}

void AHG_Player::MulticastRPCEquipItemToSocket_Implementation(AHG_EquipItem* ItemValue)
{
	if (ItemValue)
	{
		ItemValue->SetItemOwner(this);
		ItemValue->SetOwner(Cast<APlayerController>(this->Controller));
	}

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

void AHG_Player::ServerRPC_InitCharacter_Implementation(AHG_Player* Value)
{
	Multicast_InitCharacter(Value);
}
void AHG_Player::Multicast_InitCharacter_Implementation(AHG_Player* Value)
{
	if (GI->Gender != 0)
	{
		Gender = GI->Gender;
	}
	if (GI->Anim)
	{
		this->GetMesh()->SetAnimInstanceClass(GI->Anim);
	}
	if (GI->SkeletalMesh)
	{
		this->GetMesh()->SetSkeletalMesh(GI->SkeletalMesh);
	}
}

void AHG_Player::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHG_Player, EquipItemList);
	DOREPLIFETIME(AHG_Player, bEquipItem);
	DOREPLIFETIME(AHG_Player, HUD);
	// 	DOREPLIFETIME(AHG_Player, CurCloth);
	// 	DOREPLIFETIME(AHG_Player, CurClothHem);
	// 	DOREPLIFETIME(AHG_Player, CurEyes);
	// 	DOREPLIFETIME(AHG_Player, CurHair);
	// 	DOREPLIFETIME(AHG_Player, CurHairPin);
	// 	DOREPLIFETIME(AHG_Player, CurPrinting);
}


