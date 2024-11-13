// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_SoundItem.h"
#include "HG_Player.h"
#include "Components/AudioComponent.h"

void AHG_SoundItem::BeginPlay()
{
	Super::BeginPlay();

	this->ItemData.ItemName = Name;
	InitItemData();

	//SetLifeSpan(0.5f);
}

void AHG_SoundItem::Use()
{
	ServerRPC_Use();
}

void AHG_SoundItem::ServerRPC_Use_Implementation()
{
	USoundWave* Sound = ItemData.Sound.LoadSynchronous();
	MulticastRPC_Use(ItemOwner,Sound);
}

void AHG_SoundItem::MulticastRPC_Use_Implementation(APawn* p_Owner, USoundWave* p_Sound)
{
	if (p_Owner)
	{
		auto* Player = Cast<AHG_Player>(p_Owner);
		if (Player)
		{
			Player->AudioComp->SetSound(p_Sound);
			Player->AudioComp->Play();
		}
	}
}
