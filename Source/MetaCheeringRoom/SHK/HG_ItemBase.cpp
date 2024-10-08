// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_ItemBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AHG_ItemBase::AHG_ItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHG_ItemBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHG_ItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHG_ItemBase::UseItem()
{
}

void AHG_ItemBase::SetItemName(FString Value)
{
	ItemName = Value;
}

FString AHG_ItemBase::GetItemName() const
{
	return ItemName;
}

void AHG_ItemBase::SetItemIcon(UTexture2D* Value)
{
	ItemIcon = Value;
}

UTexture2D* AHG_ItemBase::GetItemIcon() const
{
	return ItemIcon;
}

void AHG_ItemBase::SetQunatity(int32 Value)
{
	Qunatity = Value;
}

// void AHG_ItemBase::AddQunatity(int32 Value)
// {
// 	Qunatity += Value;
// }

int32 AHG_ItemBase::GetQuantity() const
{
	return Qunatity;
}

