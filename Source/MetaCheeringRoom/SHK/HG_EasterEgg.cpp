// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_EasterEgg.h"
#include "Components/BoxComponent.h"
#include "HG_Player.h"
#include "HG_GameInstance.h"
#include "HG_PlayerInventoryComponent.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AHG_EasterEgg::AHG_EasterEgg()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
}

// Called when the game starts or when spawned
void AHG_EasterEgg::BeginPlay()
{
	Super::BeginPlay();
	BoxCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AHG_EasterEgg::OnMyBeginOverlap);
	BoxCollisionComp->OnComponentEndOverlap.AddDynamic(this, &AHG_EasterEgg::OnMyEndOverlap);

	ItemData.ItemName = "Faker";
	InitItemData();
}

// Called every frame
void AHG_EasterEgg::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bStaying)
	{
		StayTime -= DeltaTime;
	}

	if (StayTime <= 0)
	{
		UE_LOG(LogTemp,Warning,TEXT("외쳐 대상혁!!"));
		EasterEggComplete();
	}
}

void AHG_EasterEgg::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto* Player = Cast<AHG_Player>(OtherActor))
	{
		OverlapPlayer = Player;
		UE_LOG(LogTemp, Warning, TEXT("대상혁"));
		bStaying = true;
	}
}

void AHG_EasterEgg::OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (auto* Player = Cast<AHG_Player>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("소하혁"));
		bStaying = false;
		StayTime = 5.0f;
	}
}

void AHG_EasterEgg::EasterEggComplete()
{
	if (OverlapPlayer)
	{
		OverlapPlayer->InventoryComp->AddtoInventory(ItemData,1);
		bStaying = false;
		UUserWidget* FakerWidget = CreateWidget<UUserWidget>(GetWorld(),WidgetFactory);
		if (FakerWidget)
		{
			FakerWidget->AddToViewport();
		}
		StayTime = 5.0f;
	}
}

void AHG_EasterEgg::InitItemData()
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

void AHG_EasterEgg::SetItemData(FItemData ItemValue)
{
	ItemData.ItemClass = ItemValue.ItemClass;
	ItemData.ItemIcon = ItemValue.ItemIcon;
	ItemData.ItemName = ItemValue.ItemName;
	ItemData.ItemPrice = ItemValue.ItemPrice;
	ItemData.ItemCategory = ItemValue.ItemCategory;
	ItemData.Montage = ItemValue.Montage;
	ItemData.Sound = ItemValue.Sound;
}



