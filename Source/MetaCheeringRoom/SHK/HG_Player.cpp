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
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "HG_KomanoDummy.h"
#include "HG_ChairCollision.h"
#include "Components/ArrowComponent.h"

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
	int32 BoneIndex = GetMesh()->GetBoneIndex(TEXT("Chest"));
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

	if (!bIsSitting)
	{
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

	input->BindAction(IA_Teleport1, ETriggerEvent::Completed, this, &AHG_Player::TeleportToStore);
	input->BindAction(IA_Teleport2, ETriggerEvent::Completed, this, &AHG_Player::TeleportToJoin);
	input->BindAction(IA_Teleport3, ETriggerEvent::Completed, this, &AHG_Player::TeleportToCreate);

	input->BindAction(IA_FullScreen, ETriggerEvent::Completed, this, &AHG_Player::ConversionFullScreen);

	input->BindAction(IA_Shake, ETriggerEvent::Started, this, &AHG_Player::ShakeHand);
	input->BindAction(IA_Shake, ETriggerEvent::Completed, this, &AHG_Player::StopHand);

	input->BindAction(IA_Sit, ETriggerEvent::Completed, this, &AHG_Player::Sit);

	input->BindAction(IA_CheerSurfing, ETriggerEvent::Completed, this, &AHG_Player::CheerSurfing);

}

void AHG_Player::CheerSurfing()
{
	ServerRPC_SetCheerSurfingState();
}

void AHG_Player::Sit()
{
	if (!bIsSitting)
	{
		if (DetectChair != nullptr)
		{
			FVector ChairLoc = DetectChair->GetActorLocation();
			SetActorLocation(ChairLoc);
			FVector ChairDir = DetectChair->GetActorRightVector();
			SetActorRotation(ChairDir.Rotation());
			SpringArmComp->TargetArmLength = 0.0f;
			SpringArmComp->SetRelativeLocation(FVector(-12.0f, 0.0f, -64.0f));
			CameraComp->SetRelativeLocation(FVector(40.0f, 0.0f, 0.0f));
		}
	}
	else
	{
		SpringArmComp->TargetArmLength = 300.0f;
		SpringArmComp->SetRelativeLocation(FVector(-12.0f, 0.0f, -64.0f));
		CameraComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
	bCanMove = !bCanMove;
	ServerRPC_SetSitState();
}

void AHG_Player::ServerRPC_SetSitState_Implementation()
{
	if (!bIsSitting)
	{
		if (DetectChair)
		{
			FVector ChairLoc = DetectChair->GetActorLocation();
			SetActorLocation(ChairLoc);
			FVector ChairDir = DetectChair->GetActorRightVector();
			SetActorRotation(ChairDir.Rotation());
			SpringArmComp->TargetArmLength = 0.0f;
			SpringArmComp->SetRelativeLocation(FVector(-12.0f, 0.0f, -64.0f));
			CameraComp->SetRelativeLocation(FVector(40.0f, 0.0f, 0.0f));
			bIsSitting = true;
		}
	}
	else
	{
		bIsSitting = false;
		SpringArmComp->TargetArmLength = 300.0f;
		SpringArmComp->SetRelativeLocation(FVector(-12.0f, 0.0f, -64.0f));
		CameraComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		//DetectChair = nullptr;
	}
}

void AHG_Player::ShakeHand()
{
	ServerRPC_Shake(true);

	bCanMove = false;
}

void AHG_Player::StopHand()
{
	ServerRPC_Shake(false);

	if (bIsSitting)
	{
		bCanMove = false;
	}
	else
	{
		bCanMove = true;
	}
}

void AHG_Player::ServerRPC_Shake_Implementation(bool Value)
{
	if (HasAuthority())
	{
		bIsShaking = Value;
	}
}

void AHG_Player::ConversionFullScreen()
{
	if (nullptr == FullScreenWidget)
	{
		FullScreenWidget = CreateWidget<UUserWidget>(GetWorld(), FullScreenClass);
	}
	if (FullScreenWidget)
	{
		if (!bToggle)
		{
			FullScreenWidget->AddToViewport();
			PC->SetShowMouseCursor(true);
			bCanMove = false;
			bToggle = !bToggle;
		}
		else
		{
			FullScreenWidget->RemoveFromParent();
			PC->SetShowMouseCursor(false);
			PC->SetInputMode(FInputModeGameOnly());
			bToggle = !bToggle;
			bCanMove = true;
		}
	}

}

void AHG_Player::TeleportToStore()
{
	//(X=-867.240674,Y=927.178242,Z=-0.707882)
	UGameplayStatics::PlaySound2D(GetWorld(), TeleportSound);
	SetActorLocation(FVector(-867.240674f, 927.178242f, 0.707882f));
}

void AHG_Player::TeleportToJoin()
{
	//(X = -743.435962, Y = 2559.526662, Z = -18.354637)
	UGameplayStatics::PlaySound2D(GetWorld(), TeleportSound);
	SetActorLocation(FVector(-677.120918f, 2559.526662f, -18.354637f));
}

void AHG_Player::TeleportToCreate()
{
	//(X=1040.000000,Y=1230.000000,Z=0.000000)
	UGameplayStatics::PlaySound2D(GetWorld(), TeleportSound);
	SetActorLocation(FVector(1040.0f, 1230.0f, 0.0f));
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
	if (!bIsSitting && !bCanMove) return;

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

			UGameplayStatics::PlaySound2D(GetWorld(), UIPopUpSound);

			InventoryWidget->PlayAppearAnimation(true);
			HUD->PlayAppearAnimation(false);
			HUD->StopInventoryAnimation();
			InventoryWidget->InitInventoryUI();

			PC->SetShowMouseCursor(true);
			bToggle = !bToggle;
			bCanMove = false;
		}
		else
		{
			UGameplayStatics::PlaySound2D(GetWorld(), UIPopUpSound);
			InventoryWidget->PlayAppearAnimation(false);
			HUD->PlayAppearAnimation(true);

			FLatentActionInfo LatentInfo;
			LatentInfo.Linkage = 0;
			LatentInfo.UUID = GetUniqueID();
			LatentInfo.ExecutionFunction = FName("RemoveInventory");
			LatentInfo.CallbackTarget = this;

			UKismetSystemLibrary::Delay(GetWorld(), 0.2f, LatentInfo);

			PC->SetShowMouseCursor(false);
			bToggle = !bToggle;
			bCanMove = true;
		}
	}
}

void AHG_Player::RemoveInventory()
{
	InventoryWidget->RemoveFromParent();
}

void AHG_Player::StartFeverTime()
{
	ServerRPC_StartFeverTime();
}

void AHG_Player::ServerRPC_StartFeverTime_Implementation()
{
	Multicast_StartFeverTime();
}

void AHG_Player::Multicast_StartFeverTime_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(FeverTimeHandle, this, &AHG_Player::BlingBling, 0.1f, true);
}

void AHG_Player::EndFeverTime()
{
	// bFeverTime = false;
	ServerRPC_EndFeverTime();
}

void AHG_Player::ServerRPC_EndFeverTime_Implementation()
{
	Multicast_EndFeverTime();
}

void AHG_Player::Multicast_EndFeverTime_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(FeverTimeHandle);

	if (DynamicMaterial_HairPin)
	{
		DynamicMaterial_HairPin->SetVectorParameterValue("CustomColor", GI->CurHairPin);
		DynamicMaterial_HairPin->SetVectorParameterValue("EmissiveColor", 0 * GI->CurHairPin);
	}
	if (DynamicMaterial_ClothColor)
	{
		DynamicMaterial_ClothColor->SetVectorParameterValue("CustomColor", GI->CurClothHem);
		DynamicMaterial_ClothColor->SetVectorParameterValue("EmissiveColor", 0 * GI->CurClothHem);
	}
}

void AHG_Player::BlingBling()
{
	FLinearColor RandColor;
	if (!bToggle3)
	{
		RandColor = FLinearColor::Red;
	}
	else
	{
		RandColor = FLinearColor::Yellow;
	}
	bToggle3 = !bToggle3;

	if (DynamicMaterial_HairPin)
	{
		DynamicMaterial_HairPin->SetVectorParameterValue("EmissiveColor", 9.9 * RandColor);
	}
	if (DynamicMaterial_ClothColor)
	{
		DynamicMaterial_ClothColor->SetVectorParameterValue("EmissiveColor", 9.9 * RandColor);
	}
}

void AHG_Player::SetCheerSurfingState()
{
	bIsCheerSurfing = false;

	if (My_CheeringStick)
	{
		My_CheeringStick->ApplyChange(FLinearColor::Black, false, 0.1f);
	}
}

void AHG_Player::ServerRPC_SetCheerSurfingState_Implementation()
{
	if (bIsSitting)
	{
		bIsCheerSurfing = true;

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), CSClass, CheerSticks);

		for (auto* CheerStick : CheerSticks)
		{
			auto* C_CheerStick = Cast<AHG_CheeringStick>(CheerStick);
			if (C_CheerStick->ItemOwner == Controller->GetPawn())
			{
				if (C_CheerStick)
				{
					My_CheeringStick = C_CheerStick;
					C_CheerStick->ApplyChange(FLinearColor::Red, false, 100);
				}
			}
		}

		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, this, &AHG_Player::SetCheerSurfingState, 0.5f, false);
	}
}

void AHG_Player::Emotion()
{
	auto* Dummy = UGameplayStatics::GetActorOfClass(GetWorld(), KomanoDummyClass);
	if (auto* K_Dummy = Cast<AHG_KomanoDummy>(Dummy))
	{
		K_Dummy->CheerSurfing();
	}

	if (bEquipItem)
	{
		auto* RCSWidget = CreateWidget<UHG_RemoteCS>(GetWorld(), RCSClass);
		if (RCSWidget)
		{

			RCSWidget->AddToViewport();


			UGameplayStatics::PlaySound2D(GetWorld(), UIPopUpSound2);
			RCSWidget->PlayAppearAnimation(true);
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

			UGameplayStatics::PlaySound2D(GetWorld(), UIPopUpSound2);

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

	ItemValue->SetActorHiddenInGame(false);
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
			mesh->SetHiddenInGame(false);
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
				HUD->UpdateHUD(0);
				HUD->SetPointText();
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
		DynamicMaterial_ClothColor->SetVectorParameterValue("EmissiveColor", 0 * ClothHem);
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
		DynamicMaterial_HairPin->SetVectorParameterValue("EmissiveColor", 0 * HairPin);
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

			UGameplayStatics::PlaySound2D(GetWorld(), UIPopUpSound2);

			CustomUI->PlayAppearAnimation(true);
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
		EItem->SetActorHiddenInGame(true);
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
	DOREPLIFETIME(AHG_Player, bIsSitting);
	DOREPLIFETIME(AHG_Player, bIsShaking);
	DOREPLIFETIME(AHG_Player, bIsCheerSurfing);
}


