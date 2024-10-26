// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_EmojiItemBase.h"

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
	}
	
}

void AHG_EmojiItemBase::ChangeMeshMaterial()
{
	UMaterialInterface* NewMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/SHK/Material/M_Emoji.M_Emoji"));
	if (NewMaterial && MeshComp)
	{
		MeshComp->SetMaterial(0, NewMaterial);
	}
}

void AHG_EmojiItemBase::ChangeMaterialTexture()
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
