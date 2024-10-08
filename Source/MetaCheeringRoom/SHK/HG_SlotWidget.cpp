// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_SlotWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"

void UHG_SlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button_InventorySlot->OnClicked.AddDynamic(this, &UHG_SlotWidget::OnButtonClicked);
}

void UHG_SlotWidget::OnButtonClicked()
{
	
}

void UHG_SlotWidget::SetItemIcon(UTexture2D* Icon)
{
	if (Icon != nullptr)
	{
		Img_ItemIcon->SetBrushFromTexture(Icon);
	}
}
