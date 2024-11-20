// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_CustomUI.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"
#include "HG_Player.h"

void UHG_CustomUI::NativeConstruct()
{
	PNGS = TArray<UTexture2D*>();

	PNGS.Add(LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/SHK/Texture/Charater/Custom/T1Basic.T1Basic'")));
	PNGS.Add(LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/SHK/Texture/Charater/Custom/Faker1.Faker1'")));
	PNGS.Add(LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/SHK/Texture/Charater/Custom/Faker2.Faker2'")));
	PNGS.Add(LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/SHK/Texture/Charater/Custom/SadBee.SadBee'")));
	PNGS.Add(LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/SHK/Texture/Charater/Custom/T1FiveStar.T1FiveStar'")));
	PNGS.Add(LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/SHK/Texture/Charater/Custom/TeemoThumb.TeemoThumb'")));
	PNGS.Add(LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/SHK/Texture/Charater/Custom/WhatTheCrab.WhatTheCrab'")));
	PNGS.Add(LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/SHK/Texture/Charater/Custom/Zeus.Zeus'")));
	PNGS.Add(LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/SHK/Texture/Charater/Custom/T1.T1'")));

	if (!Btn_Next->OnClicked.IsBound())
	{
		Btn_Next->OnClicked.AddDynamic(this, &UHG_CustomUI::SetNextImage);
	}

	if (!Btn_Commit->OnClicked.IsBound())
	{
		Btn_Commit->OnClicked.AddDynamic(this, &UHG_CustomUI::SetCustomInfo);
	}

	if (!Btn_Red->OnClicked.IsBound())Btn_Red->OnClicked.AddDynamic(this, &UHG_CustomUI::OnClickRed);
	if (!Btn_Orange->OnClicked.IsBound())Btn_Orange->OnClicked.AddDynamic(this, &UHG_CustomUI::OnClickOrange);
	if (!Btn_Yellow->OnClicked.IsBound())Btn_Yellow->OnClicked.AddDynamic(this, &UHG_CustomUI::OnClicYellow);
	if (!Btn_Green->OnClicked.IsBound())Btn_Green->OnClicked.AddDynamic(this, &UHG_CustomUI::OnClickGreen);
	if (!Btn_Blue->OnClicked.IsBound())Btn_Blue->OnClicked.AddDynamic(this, &UHG_CustomUI::OnClickBlue);
	if (!Btn_Sodomy->OnClicked.IsBound())Btn_Sodomy->OnClicked.AddDynamic(this, &UHG_CustomUI::OnClickSodomy);
	if (!Btn_Purple->OnClicked.IsBound())Btn_Purple->OnClicked.AddDynamic(this, &UHG_CustomUI::OnClickPurple);
	if (!Btn_White->OnClicked.IsBound())Btn_White->OnClicked.AddDynamic(this, &UHG_CustomUI::OnClickWhite);
	if (!Btn_Black->OnClicked.IsBound())Btn_Black->OnClicked.AddDynamic(this, &UHG_CustomUI::OnClickBlack);
	if (!Btn_Grey->OnClicked.IsBound())Btn_Grey->OnClicked.AddDynamic(this, &UHG_CustomUI::OnClickGrey);

	Img_Printing->SetBrushFromTexture(PNGS[0]);
	CurIdx = 0;
	Count = 0;
}

void UHG_CustomUI::BeginDestroy()
{
	Super::BeginDestroy();

	auto* PC = Cast<APlayerController>(GetOwningPlayer());
	if (PC)
	{
		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
	}
}

void UHG_CustomUI::SetNextImage()
{
	if (PNGS.Num() != 0 && nullptr != Img_Printing)
	{
		CurIdx++;
		if (CurIdx >= PNGS.Num())
		{
			CurIdx = 0;
		}
		Img_Printing->SetBrushFromTexture(PNGS[CurIdx]);
	}
}

void UHG_CustomUI::SetCustomInfo()
{
	if (Count == 0)
	{
		ClothColor = SelectedColor;
		ClothPrinting = PNGS[CurIdx];
		Count++;
		SelectedColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

		TB_Printing->SetVisibility(ESlateVisibility::Hidden);
		Img_Printing->SetVisibility(ESlateVisibility::Hidden);
		Btn_Next->SetVisibility(ESlateVisibility::Hidden);

		TB_Target_Color->SetText(FText::FromString(TEXT("옷단 색깔")));
	}
	else if (Count == 1)
	{
		ClothHemColor = SelectedColor;
		Count++;
		SelectedColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

		TB_Target_Color->SetText(FText::FromString(TEXT("머리 색깔")));
	}
	else if (Count == 2)
	{
		HairColor = SelectedColor;
		Count++;
		SelectedColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

		TB_Target_Color->SetText(FText::FromString(TEXT("머리핀 색깔")));
	}
	else if (Count == 3)
	{
		HairPinColor = SelectedColor;
		Count++;
		SelectedColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

		TB_Target_Color->SetText(FText::FromString(TEXT("눈 색깔")));
	}
	else if (Count == 4)
	{
		EyesColor = SelectedColor;
		Count = 0;
		SelectedColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

		TB_Printing->SetVisibility(ESlateVisibility::Visible);
		Img_Printing->SetVisibility(ESlateVisibility::Visible);
		Btn_Next->SetVisibility(ESlateVisibility::Visible);

		TB_Target_Color->SetText(FText::FromString(TEXT("옷 색깔")));

		RemoveFromParent();

		auto* PC = Cast<APlayerController>(GetOwningPlayer());
		if (PC)
		{
			PC->SetShowMouseCursor(false);
			PC->SetInputMode(FInputModeGameOnly());
			auto* Player = Cast<AHG_Player>(PC->GetPawn());
			if (Player)
			{
				Player->ServerRPC_ApplyCustom(ClothColor, ClothPrinting, ClothHemColor, EyesColor, HairColor, HairPinColor);
			}
		}
	}
}

void UHG_CustomUI::OnClickRed()
{
	SelectedColor = FLinearColor(1.0f, 0.0f, 0.0f);
}

void UHG_CustomUI::OnClickOrange()
{
	SelectedColor = FLinearColor(0.730461f, 0.552011f, 0.356400f,1.0f);
}

void UHG_CustomUI::OnClicYellow()
{
	SelectedColor = FLinearColor(1.0f, 0.887923f, 0.068478f);
}

void UHG_CustomUI::OnClickGreen()
{
	SelectedColor = FLinearColor(0.0f, 0.5f, 0.0f);
}

void UHG_CustomUI::OnClickBlue()
{
	SelectedColor = FLinearColor(0.084376f, 0.401978f, 0.597202f);
}

void UHG_CustomUI::OnClickSodomy()
{
	SelectedColor = FLinearColor(0.037975f, 0.0f, 0.182292f);
}

void UHG_CustomUI::OnClickPurple()
{
	SelectedColor = FLinearColor(0.68f, 0.0f, 1.0f);
}

void UHG_CustomUI::OnClickWhite()
{
	SelectedColor = FLinearColor(1.0f, 1.0f, 1.0f);
}

void UHG_CustomUI::OnClickBlack()
{
	SelectedColor = FLinearColor(0.0f, 0.0f, 0.0f);
}

void UHG_CustomUI::OnClickGrey()
{
	SelectedColor = FLinearColor(0.5f, 0.5f, 0.5f);
}

void UHG_CustomUI::PlayAppearAnimation(bool Play_Forward)
{
	if (Appear)
	{
		PlayAnimation(Appear);
	}
}
