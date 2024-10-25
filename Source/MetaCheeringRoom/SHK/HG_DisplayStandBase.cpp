// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_DisplayStandBase.h"
#include "HG_GameInstance.h"
#include "HG_ItemPurchaseWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "HG_Player.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AHG_DisplayStandBase::AHG_DisplayStandBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	BoxComp->SetGenerateOverlapEvents(true);
	BoxComp->SetCollisionProfileName(TEXT("DisplayStand"));

	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidget->SetupAttachment(RootComponent);
	InteractionWidget->SetVisibility(false);
}

// Called when the game starts or when spawned
void AHG_DisplayStandBase::BeginPlay()
{
	Super::BeginPlay();

	ItemData.ItemName = DisplayItemName;
	InitItemData();
}

// Called every frame
void AHG_DisplayStandBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DetectActor != nullptr)
	{
		FVector target = Cast<APlayerController>(DetectActor->Controller)->PlayerCameraManager->GetCameraLocation();
		FVector dir = target - InteractionWidget->GetComponentLocation();
		dir.Normalize();
		float Temp = dir.Z;
		dir.Z = 0;

		FRotator rot = dir.ToOrientationRotator();
		InteractionWidget->SetWorldRotation(rot);
		auto* Player = Cast<AHG_Player>(DetectActor);
		if (Player)
		{
			InteractionWidget->SetWorldLocation(Player->LookingPoint);
		}
	}
}

void AHG_DisplayStandBase::InitItemData()
{
	TArray<FItemData*> AllRows;
	GI = Cast<UHG_GameInstance>(GetWorld()->GetGameInstance());
	if (GI)
	{
		GI->ItemDataTable->GetAllRows(TEXT(""), AllRows);
		for (auto Row : AllRows)
		{
			if (Row->ItemName == ItemData.ItemName)
			{
				SetItemData(*Row);
				break;
			}
		}
	}
}

void AHG_DisplayStandBase::SetItemData(FItemData ItemValue)
{
	ItemData.ItemClass = ItemValue.ItemClass;
	ItemData.ItemIcon = ItemValue.ItemIcon;
	ItemData.ItemName = ItemValue.ItemName;
	ItemData.ItemPrice = ItemValue.ItemPrice;
	ItemData.ItemCategory = ItemValue.ItemCategory;
}

void AHG_DisplayStandBase::Detected(bool Value, APawn* p_DetectActor)
{
	InteractionWidget->SetVisibility(Value);
	DetectActor = p_DetectActor;
	auto* Player = Cast<AHG_Player>(DetectActor);
	if (Player)
	{
		Player->bDetectStand = Value;
		if (Value)
		{
			Player->CameraComp->SetRelativeRotation(FRotator(0.f,0.f,0.f));
			Player->CameraComp->SetRelativeLocation(FVector(0.f,0.f,-20.0f));
		}
		else
		{
			Player->SpringArmComp->SetRelativeLocation(FVector(0.f,0.f,20.0f));
			Player->CameraComp->SetRelativeRotation(FRotator(0.f, -20.f, 0.f));
			Player->CameraComp->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
			InteractionWidget->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		}
	}
}
