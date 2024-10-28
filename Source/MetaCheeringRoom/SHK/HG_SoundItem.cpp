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
	MulticastRPC_Use();
}

void AHG_SoundItem::MulticastRPC_Use_Implementation()
{
	if (Owner)
	{
		auto* Player = Cast<AHG_Player>(Owner);
		if (Player)
		{
			UE_LOG(LogTemp,Warning,TEXT("11"));
			USoundWave* Sound = ItemData.Sound.LoadSynchronous();
			Player->AudioComp->SetSound(Sound);
			Player->AudioComp->Play();
		}
	}
}
