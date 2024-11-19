// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_HUD.h"
#include "Components/TextBlock.h"
#include "HG_Player.h"
#include "HG_PlayerGoodsComponent.h"

void UHG_HUD::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHG_HUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);

	SetPointText();
}

void UHG_HUD::SetPointText()
{
	auto* Player = Cast<AHG_Player>(GetOwningPlayer()->GetPawn());
	if (Player)
	{
		TB_Point->SetText(FText::AsNumber(Player->GoodsComp->GetGold()));
	}
}

void UHG_HUD::UpdateHUD(FString Value)
{
	TB_RemoteCS->SetText(FText::FromString(Value));
}
