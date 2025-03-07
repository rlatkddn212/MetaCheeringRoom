// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_PlayerGoodsComponent.h"
#include "HG_Player.h"
#include "HG_GameInstance.h"
#include "HG_HUD.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHG_PlayerGoodsComponent::UHG_PlayerGoodsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHG_PlayerGoodsComponent::BeginPlay()
{
	Super::BeginPlay();

	CompOwner = Cast<AHG_Player>(GetOwner());
}


// Called every frame
void UHG_PlayerGoodsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHG_PlayerGoodsComponent::SetGold(int32 Value)
{
	Gold = Value;
	CompOwner->GI->CurrentGold = Gold;
	if (CompOwner->HUD)
	{
		CompOwner->HUD->SetPointText();
	}
}

int32 UHG_PlayerGoodsComponent::GetGold() const
{
	return Gold;
}

int32 UHG_PlayerGoodsComponent::AddGold(int32 Value)
{
	SetGold(FMath::Clamp(Gold + Value, 0, MAX_GOLD));
	CompOwner->GI->CurrentGold = Gold;
	CompOwner->HUD->SetPointText();
	if(CompOwner->HUD->GetCoin)
	CompOwner->HUD->PlayAnimation(CompOwner->HUD->GetCoin);

	if (CompOwner->bOnFullScreen == false && CompOwner->bGetCoinSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), CompOwner->GetPointSound);
	}
	
	return Gold;
}

int32 UHG_PlayerGoodsComponent::SubGold(int32 Value)
{
	SetGold(FMath::Clamp(Gold - Value, 0, MAX_GOLD));
	CompOwner->GI->CurrentGold = Gold;
	CompOwner->HUD->SetPointText();
	return Gold;
}

int32 UHG_PlayerGoodsComponent::GoldLerp(int32 p_TargetGold,float OneTick)
{
	GetWorld()->GetTimerManager().SetTimer(handle,this,&UHG_PlayerGoodsComponent::Add10Point, OneTick,true);

	TargetGold = p_TargetGold;

	return Gold;
}

void UHG_PlayerGoodsComponent::Add10Point()
{
	if (CompOwner->bOnFullScreen == false)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), CompOwner->GetPointSound);
	}

	CompOwner->HUD->SetAnimPointText(Gold);

	Gold+= 10;

	CompOwner->HUD->SetPointText();

	if (CompOwner->HUD->TBSlide)
	{
		CompOwner->HUD->PlayAnimation(CompOwner->HUD->TBSlide);
	}

	if (Gold >= TargetGold)
	{
		GetWorld()->GetTimerManager().ClearTimer(handle);
	}
}

