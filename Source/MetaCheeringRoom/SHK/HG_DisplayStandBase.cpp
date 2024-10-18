// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_DisplayStandBase.h"
#include "HG_GameInstance.h"
#include "HG_ItemPurchaseWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"

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

	ItemData.ItemName = "FireCracker";
	InitItemData();
}

// Called every frame
void AHG_DisplayStandBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DetectActor != nullptr)
	{
		// 사용자의 카메라를 찾고
		FVector target = Cast<APlayerController>(DetectActor->Controller)->PlayerCameraManager->GetCameraLocation();
		// 그 카메라를 바라보는 방향을 만들어서
		FVector dir = target - InteractionWidget->GetComponentLocation();
		dir.Normalize();

		// HPComp를 회전하고싶다.(Billboard 기법)
		FRotator rot = dir.ToOrientationRotator();
		InteractionWidget->SetWorldRotation(rot);
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
}
