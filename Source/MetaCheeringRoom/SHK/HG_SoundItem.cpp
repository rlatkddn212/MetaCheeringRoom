// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_SoundItem.h"
#include "HG_Player.h"
#include "Components/AudioComponent.h"

void AHG_SoundItem::BeginPlay()
{
	Super::BeginPlay();

	this->ItemData.ItemName = Name;
	InitItemData();

}

void AHG_SoundItem::Use()
{
	if (Owner)
	{
		auto* Player = Cast<AHG_Player>(Owner);
		if (Player)
		{
			Player->AudioComp->SetSound(this->ItemData.Sound);
			Player->AudioComp->Play();
		}
	}
}
