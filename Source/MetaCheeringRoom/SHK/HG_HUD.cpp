// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_HUD.h"
#include "Components/TextBlock.h"
#include "HG_Player.h"
#include "HG_PlayerGoodsComponent.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"

void UHG_HUD::SetPointText()
{
	if (GetOwningPlayer())
	{
		auto* Player = Cast<AHG_Player>(GetOwningPlayer()->GetPawn());
		if (Player)
		{
			TB_Point->SetText(FText::AsNumber(Player->GoodsComp->GetGold()));
		}
	}
}

void UHG_HUD::UpdateHUD(int32 Value)
{
	SetPointText();
	if (Value == 0)
	{
		WS_Border->SetActiveWidgetIndex(0);
	}
	else
	{
		WS_Border->SetActiveWidgetIndex(1);
	}
}

void UHG_HUD::PlayAppearAnimation(bool Play_Forward)
{
	if (IsAnimationPlaying(Appear))
	{
		StopAnimation(Appear);
		StopAnimation(Disappear);
	}

	if (IsAnimationPlaying(Disappear))
	{
		StopAnimation(Disappear);
	}

	if (!Appear || !Disappear) return;

	if (Play_Forward)
	{
		PlayAnimation(Appear);
	}
	else
	{
		PlayAnimation(Disappear);
	}
}

void UHG_HUD::PlayInventoryAnimation()
{
	if (NewItem)
	{
		PlayAnimation(NewItem, 0.0f, 0);
	}
}

void UHG_HUD::StopInventoryAnimation()
{
	if (IsAnimationPlaying(NewItem))
	{
		StopAnimation(NewItem);
	}
}


