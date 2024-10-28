// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_EmojiItemBase.h"
#include "Net/UnrealNetwork.h"

AHG_EmojiItemBase::AHG_EmojiItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);
}

void AHG_EmojiItemBase::BeginPlay()
{
	Super::BeginPlay();

	this->ItemData.ItemName = Name;
	InitItemData();
	
	SetLifeSpan(4.0f);
}

void AHG_EmojiItemBase::Use()
{
	ChangeMeshMaterial();
	ChangeMaterialTexture();
}

void AHG_EmojiItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Owner)
	{
		SetActorLocation(Owner->GetActorLocation() + FVector(0.0f, 0.0f, 120.0f));
		SetActorRelativeRotation(Owner->GetActorRotation() + FRotator(90.0f,0.0f,0.0f));
		Loc = GetActorLocation();
		Rot = GetActorRotation();
	}
	
}

void AHG_EmojiItemBase::ServerRPC_ChangeMeshMaterial_Implementation()
{
	MulticastRPC_ChangeMeshMaterial();
}

void AHG_EmojiItemBase::MulticastRPC_ChangeMeshMaterial_Implementation()
{
	UMaterialInterface* NewMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/SHK/Material/M_Emoji.M_Emoji"));
	if (NewMaterial && MeshComp)
	{
		MeshComp->SetMaterial(0, NewMaterial);
	}
}

void AHG_EmojiItemBase::ChangeMeshMaterial()
{
	ServerRPC_ChangeMeshMaterial();
}

void AHG_EmojiItemBase::ServerRPC_ChangeMaterialTexture_Implementation()
{
	MulticastRPC_ChangeMaterialTexture();
}

void AHG_EmojiItemBase::MulticastRPC_ChangeMaterialTexture_Implementation()
{
	if (MeshComp)
	{
		UMaterialInstanceDynamic* DynamicMaterial = MeshComp->CreateAndSetMaterialInstanceDynamic(0);

		if (DynamicMaterial && ItemData.ItemIcon)
		{
			DynamicMaterial->SetTextureParameterValue(FName("BaseTexture"), ItemData.ItemIcon);
		}
	}
}

void AHG_EmojiItemBase::ChangeMaterialTexture()
{
	ServerRPC_ChangeMaterialTexture();
}

void AHG_EmojiItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHG_EmojiItemBase, Rot);
	DOREPLIFETIME(AHG_EmojiItemBase, Loc);
}