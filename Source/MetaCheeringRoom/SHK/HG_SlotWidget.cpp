// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_SlotWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Engine/DataTable.h"
#include "InventoryWidget.h"
#include "HG_ItemBase.h"
#include "Components/TextBlock.h"
#include "HG_GameInstance.h"
#include "HG_Player.h"

void UHG_SlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!(Button_InventorySlot->OnHovered.IsBound()))
	{
		Button_InventorySlot->OnHovered.AddDynamic(this, &UHG_SlotWidget::OnButtonHovered);
	}
	if (!(Button_InventorySlot->OnUnhovered.IsBound()))
	{
		Button_InventorySlot->OnUnhovered.AddDynamic(this, &UHG_SlotWidget::OnButtonUnHovered);
	}

	if (!(Button_InventorySlot->OnPressed.IsBound()))
	{
		Button_InventorySlot->OnPressed.AddDynamic(this, &UHG_SlotWidget::OnButtonClicked);
	}

	NormalStyle = Button_InventorySlot->WidgetStyle;

	MyIndex = Owner->GetSlotIndexInWB(this);
}

void UHG_SlotWidget::SetItemName()
{
	TB_ItemName->SetText(FText::FromString(SlotInfo.ItemInfo.ItemName));
}

void UHG_SlotWidget::OnButtonClicked()
{
	Owner->SelectedSlot = this;
	Owner->UseItem();

	if (!Button_InventorySlot) return;

	FSlateBrush PressedBrush;
	PressedBrush.SetResourceObject(LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/SHK/Texture/Inventory/slot.slot'")));
	PressedBrush.ImageSize = FVector2D(78.0f, 78.0f); 
	PressedBrush.TintColor = FSlateColor::UseForeground();
	PressedBrush.DrawAs = ESlateBrushDrawType::RoundedBox;
	
	PressedBrush.OutlineSettings.CornerRadii = FVector4(4.0f, 4.0f, 4.0f, 4.0f);
	PressedBrush.OutlineSettings.Color = FLinearColor(0.72f,0.72f,0.72f,1.0f);
	PressedBrush.OutlineSettings.Width = 1.0f;
	PressedBrush.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
	PressedBrush.OutlineSettings.bUseBrushTransparency = true;

	FButtonStyle Pressed = Button_InventorySlot->WidgetStyle;

	Pressed.SetPressed(PressedBrush);

	Button_InventorySlot->SetStyle(Pressed);
	
	Owner->RemoveFromParent();

	if (auto OwningPlayer = Cast<AHG_Player>(Owner->GetOwningPlayer()->GetPawn()))
	{
		OwningPlayer->PC->SetShowMouseCursor(false);
		OwningPlayer->PC->SetInputMode(FInputModeGameOnly());
		OwningPlayer->bCanMove = true;
		OwningPlayer->bToggle = !OwningPlayer->bToggle;
	}
}

void UHG_SlotWidget::OnButtonHovered()
{
	TB_ItemName->SetVisibility(ESlateVisibility::HitTestInvisible);
	SetItemName();
}

void UHG_SlotWidget::OnButtonUnHovered()
{
	TB_ItemName->SetVisibility(ESlateVisibility::Hidden);
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
	SlotInfo.ItemInfo.ItemCategory = p_Slot.ItemInfo.ItemCategory;
	SlotInfo.ItemInfo.Woman_Montage = p_Slot.ItemInfo.Woman_Montage;
	SlotInfo.ItemInfo.Man_Montage = p_Slot.ItemInfo.Man_Montage;
	SlotInfo.Quantity = p_Slot.Quantity;
}

void UHG_SlotWidget::SetOwner(UInventoryWidget* p_Owner)
{
	if (p_Owner != nullptr)
	{
		Owner = p_Owner;
	}
}


