// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_SlotWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Engine/DataTable.h"
#include "InventoryWidget.h"
#include "HG_ItemBase.h"

void UHG_SlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button_InventorySlot->OnClicked.AddDynamic(this, &UHG_SlotWidget::OnButtonClicked);

}

void UHG_SlotWidget::OnButtonClicked()
{
	Owner->SelectedSlot = this;
	Owner->DIsplaySelectedItemInfo();
}

void UHG_SlotWidget::SetItemIcon()
{
	if (SlotInfo.ItemInfo.ItemIcon != nullptr)
	{
		Img_ItemIcon->SetBrushFromTexture(SlotInfo.ItemInfo.ItemIcon);
	}
}

void UHG_SlotWidget::InitSlot(FSlotStruct p_Slot)
{
	SlotInfo.ItemInfo.ItemIcon = p_Slot.ItemInfo.ItemIcon;
	SlotInfo.ItemInfo.ItemName = p_Slot.ItemInfo.ItemName;
	SlotInfo.ItemInfo.ItemPrice = p_Slot.ItemInfo.ItemPrice;
	SlotInfo.ItemInfo.ItemClass = p_Slot.ItemInfo.ItemClass;
	SlotInfo.Quantity = p_Slot.Quantity;
}

void UHG_SlotWidget::SetOwner(UInventoryWidget* p_Owner)
{
	if (p_Owner != nullptr)
	{
		Owner = p_Owner;
	}
}


