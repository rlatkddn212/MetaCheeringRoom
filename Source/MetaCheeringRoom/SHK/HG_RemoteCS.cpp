// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_RemoteCS.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "HG_CheeringStick.h"
#include "HG_Player.h"
#include "Components/EditableText.h"
#include "Misc/DefaultValueHelper.h"
#include "Components/CheckBox.h"

void UHG_RemoteCS::NativeConstruct()
{
	Btn_Red->OnClicked.AddDynamic(this,&UHG_RemoteCS::OnClickRed);
	Btn_Orange->OnClicked.AddDynamic(this,&UHG_RemoteCS::OnClickOrange);
	Btn_Yellow->OnClicked.AddDynamic(this,&UHG_RemoteCS::OnClicYellow);
	Btn_Green->OnClicked.AddDynamic(this,&UHG_RemoteCS::OnClickGreen);
	Btn_Blue->OnClicked.AddDynamic(this,&UHG_RemoteCS::OnClickBlue);
	Btn_Sodomy->OnClicked.AddDynamic(this,&UHG_RemoteCS::OnClickSodomy);
	Btn_Purple->OnClicked.AddDynamic(this,&UHG_RemoteCS::OnClickPurple);
	Btn_White->OnClicked.AddDynamic(this,&UHG_RemoteCS::OnClickWhite);
	Btn_Black->OnClicked.AddDynamic(this,&UHG_RemoteCS::OnClickBlack);
	Btn_Grey->OnClicked.AddDynamic(this,&UHG_RemoteCS::OnClickGrey);
	Btn_Commit->OnClicked.AddDynamic(this,&UHG_RemoteCS::OnClickCommit);

	ET_Intesity->OnTextCommitted.AddDynamic(this,&UHG_RemoteCS::OnTextEnter);

	CB_Bling->OnCheckStateChanged.AddDynamic(this, &UHG_RemoteCS::OnCheckBoxChanged);
}

void UHG_RemoteCS::OnClickRed()
{
	Color = FLinearColor(1.0f,0.0f,0.0f);
}

void UHG_RemoteCS::OnClickOrange()
{
	Color = FLinearColor(1.0f, 0.64f, 0.0f);
}

void UHG_RemoteCS::OnClicYellow()
{
	Color = FLinearColor(1.0f, 1.0f, 0.0f);
}

void UHG_RemoteCS::OnClickGreen()
{
	Color = FLinearColor(0.0f, 0.5f, 0.0f);
}

void UHG_RemoteCS::OnClickBlue()
{
	Color = FLinearColor(0.0f, 0.0f, 1.0f);
}

void UHG_RemoteCS::OnClickSodomy()
{
	Color = FLinearColor(0.037975f, 0.0f, 0.182292f);
}

void UHG_RemoteCS::OnClickPurple()
{
	Color = FLinearColor(0.68f, 0.0f, 1.0f);
}

void UHG_RemoteCS::OnClickWhite()
{
	Color = FLinearColor(1.0f, 1.0f, 1.0f);
}

void UHG_RemoteCS::OnClickBlack()
{
	Color = FLinearColor(0.0f, 0.0f, 0.0f);
}

void UHG_RemoteCS::OnClickGrey()
{
	Color = FLinearColor(0.5f, 0.5f, 0.5f);
}

void UHG_RemoteCS::OnClickCommit()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), CSClass, CheerSticks);
	for (auto* CheerStick : CheerSticks)
	{
		auto* C_CheerSticks = Cast<AHG_CheeringStick>(CheerStick);
		if (C_CheerSticks)
		{
			C_CheerSticks->ApplyChange(Color,bChecked,Intensity);
		} 
	}

	RemoveFromParent();

	if (Onwer->PC)
	{
		Onwer->PC->SetShowMouseCursor(false);

		FInputModeGameOnly InputMode;
		Onwer->PC->SetInputMode(InputMode);
	}

}

void UHG_RemoteCS::OnTextEnter(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter || ETextCommit::OnUserMovedFocus)
	{
		FDefaultValueHelper::ParseFloat(Text.ToString() , Intensity);
	}
}

void UHG_RemoteCS::OnCheckBoxChanged(bool bIsChecked)
{
	bChecked = bIsChecked;
}

void UHG_RemoteCS::SetOnwer(AHG_Player* Value)
{
	Onwer = Value;
}


