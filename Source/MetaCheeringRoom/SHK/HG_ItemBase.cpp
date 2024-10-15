// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_ItemBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/DataTable.h"

// Sets default values
AHG_ItemBase::AHG_ItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetGenerateOverlapEvents(true);
	BoxComp->SetCollisionProfileName(TEXT("Item"));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableObj(TEXT("/Script/Engine.DataTable'/Game/SHK/BP/DT_Item.DT_Item'"));
	if (DataTableObj.Succeeded())
	{
		ItemDataTable = DataTableObj.Object;
	}
}

// Called when the game starts or when spawned
void AHG_ItemBase::BeginPlay()
{
	Super::BeginPlay();
	SetItemName(TEXT("Green"));
	InitItemData();
}

// Called every frame
void AHG_ItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHG_ItemBase::Use()
{
}

void AHG_ItemBase::SetItemName(FString Value)
{
	ItemData.ItemName = Value;
}

FString AHG_ItemBase::GetItemName()
{
	return ItemData.ItemName;
}

void AHG_ItemBase::SetItemIcon(UTexture2D* Value)
{
	ItemData.ItemIcon = Value;
}

UTexture2D* AHG_ItemBase::GetItemIcon()
{
	return ItemData.ItemIcon;
}

FItemData AHG_ItemBase::GetItemData()
{
	return ItemData;
}

void AHG_ItemBase::SetItemData(FItemData ItemValue)
{
	ItemData.ItemClass = this->GetClass();
	ItemData.ItemIcon = ItemValue.ItemIcon;
	ItemData.ItemName = ItemValue.ItemName;
	ItemData.ItemPrice = ItemValue.ItemPrice;
}

void AHG_ItemBase::InitItemData()
{
	
	TArray<FItemData*> AllRows;
	ItemDataTable->GetAllRows(TEXT(""), AllRows);
	for (auto Row : AllRows)
	{
		if (Row->ItemName == ItemData.ItemName)
		{
			UE_LOG(LogTemp, Warning, TEXT("Init"));
			SetItemData(*Row);
			break;
		}
	}
}
