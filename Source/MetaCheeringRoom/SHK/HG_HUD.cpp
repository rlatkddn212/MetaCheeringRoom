// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_HUD.h"
#include "Components/TextBlock.h"

void UHG_HUD::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHG_HUD::UpdateHUD(FString Value)
{
	TB_RemoteCS->SetText(FText::FromString(Value));
}
