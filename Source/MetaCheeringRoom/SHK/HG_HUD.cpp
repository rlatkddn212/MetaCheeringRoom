// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_HUD.h"
#include "Components/TextBlock.h"
#include "HG_Player.h"
#include "HG_PlayerGoodsComponent.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"

void UHG_HUD::NativeConstruct()
{
	Btn_Custom_L->OnClicked.AddDynamic(this, &UHG_HUD::CustomButton);
	Btn_Custom_R->OnClicked.AddDynamic(this, &UHG_HUD::CustomButton);

	Btn_CheerStick_L->OnClicked.AddDynamic(this, &UHG_HUD::RCSButton);
	Btn_CheerStick_R->OnClicked.AddDynamic(this, &UHG_HUD::RCSButton);

	Btn_Inventory_L->OnClicked.AddDynamic(this, &UHG_HUD::InvenButton);
	Btn_Inventory_R->OnClicked.AddDynamic(this, &UHG_HUD::InvenButton);

	Btn_Teleport1->OnClicked.AddDynamic(this, &UHG_HUD::Tele1Button);
	Btn_Teleport2->OnClicked.AddDynamic(this, &UHG_HUD::Tele2Button);
	Btn_Teleport3->OnClicked.AddDynamic(this, &UHG_HUD::Tele3Button);

	Btn_Video->OnClicked.AddDynamic(this, &UHG_HUD::Tele3Button);
	Btn_Predict->OnClicked.AddDynamic(this, &UHG_HUD::Tele3Button);
	Btn_FullScreen->OnClicked.AddDynamic(this, &UHG_HUD::Tele3Button);
	Btn_MicToggle->OnClicked.AddDynamic(this, &UHG_HUD::Tele3Button);
	Btn_Chat->OnClicked.AddDynamic(this, &UHG_HUD::Tele3Button);
}

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

void UHG_HUD::SetAnimPointText(int32 Value)
{
	if (GetOwningPlayer())
	{
		auto* Player = Cast<AHG_Player>(GetOwningPlayer()->GetPawn());
		if (Player)
		{
			TB_Animation->SetText(FText::AsNumber(Value));
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

void UHG_HUD::CustomButton()
{
	if (GetOwningPlayer())
	{
		auto* Player = Cast<AHG_Player>(GetOwningPlayer()->GetPawn());
		if (Player)
		{
			Player->PopUpCustomUI();
		}
	}
}

void UHG_HUD::RCSButton()
{
	if (GetOwningPlayer())
	{
		auto* Player = Cast<AHG_Player>(GetOwningPlayer()->GetPawn());
		if (Player)
		{
			Player->Emotion();
		}
	}
}

void UHG_HUD::InvenButton()
{
	if (GetOwningPlayer())
	{
		auto* Player = Cast<AHG_Player>(GetOwningPlayer()->GetPawn());
		if (Player)
		{
			Player->PopUpInventory();
		}
	}
}

void UHG_HUD::Tele1Button()
{
	if (GetOwningPlayer())
	{
		auto* Player = Cast<AHG_Player>(GetOwningPlayer()->GetPawn());
		if (Player)
		{
			Player->TeleportToStore();
		}
	}
}

void UHG_HUD::Tele2Button()
{
	if (GetOwningPlayer())
	{
		auto* Player = Cast<AHG_Player>(GetOwningPlayer()->GetPawn());
		if (Player)
		{
			Player->TeleportToJoin();
		}
	}
}

void UHG_HUD::Tele3Button()
{
	if (GetOwningPlayer())
	{
		auto* Player = Cast<AHG_Player>(GetOwningPlayer()->GetPawn());
		if (Player)
		{
			Player->TeleportToCreate();
		}
	}
}

void UHG_HUD::FullScreenButton()
{
	if (GetOwningPlayer())
	{
		auto* Player = Cast<AHG_Player>(GetOwningPlayer()->GetPawn());
		if (Player)
		{
			Player->ConversionFullScreen();
		}
	}
}


