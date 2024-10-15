// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "HG_Player.h"
#include "HG_PlayerInventoryComponent.h"
#include "SlotStruct.h"
#include "HG_SlotWidget.h"
#include "Components/WrapBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UInventoryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventoryWidget::NativeConstruct()
{
	//Btn_Use->OnClicked.AddDynamic();
	Btn_ThrowAway->OnClicked.AddDynamic(this, &UInventoryWidget::ThrowAwaySelectedItem);
	Btn_Use->OnClicked.AddDynamic(this, &UInventoryWidget::UseItem);
}

void UInventoryWidget::InitInventoryUI()
{
	WB_SlotList->ClearChildren();
	SelectedSlot = nullptr;
	if (this->GetOwningPlayer())
	{
		auto* OwningPlayer = Cast<AHG_Player>(this->GetOwningPlayer()->GetPawn());
		if (OwningPlayer)
		{
			for (auto slot : OwningPlayer->InventoryComp->Inventory)
			{
				UHG_SlotWidget* SlotWidget = CreateWidget<UHG_SlotWidget>(this, SlotWidgetFactory);
				if (SlotWidget)
				{
					SlotWidget->InitSlot(slot);
					SlotWidget->SetItemIcon();
					SlotWidget->SetOwner(this);
					WB_SlotList->AddChildToWrapBox(SlotWidget);
				}
			}
		}
	}
}

void UInventoryWidget::SetOwner(APawn* Player)
{
	if (Player)
	{
		this->SetOwningPlayer(Cast<APlayerController>(Player->Controller));
	}
}

void UInventoryWidget::DIsplaySelectedItemInfo()
{
	if (SelectedSlot)
	{
		Img_SelectedItem->SetBrushFromTexture(SelectedSlot->SlotInfo.ItemInfo.ItemIcon);
		TB_ItemName->SetText(FText::FromString(SelectedSlot->SlotInfo.ItemInfo.ItemName));
		TB_Price->SetText(FText::AsNumber((SelectedSlot->SlotInfo.ItemInfo.ItemPrice)));
		TB_Quantity->SetText(FText::AsNumber(SelectedSlot->SlotInfo.Quantity));
	}
	else
	{
		Img_SelectedItem->SetBrushFromTexture(nullptr);
		TB_ItemName->SetText(FText::FromString(TEXT("")));
		TB_Price->SetText(FText::FromString(TEXT("")));
		TB_Quantity->SetText(FText::FromString(TEXT("")));
	}
}

void UInventoryWidget::ThrowAwaySelectedItem()
{
	if (SelectedSlot)
	{
		int32 ChildCount = WB_SlotList->GetChildrenCount();
		for (int32 i = 0; i < ChildCount; i++)
		{
			UHG_SlotWidget* Child = Cast<UHG_SlotWidget>(WB_SlotList->GetChildAt(i));
			if (Child == SelectedSlot)
			{
				auto* Owner = Cast<AHG_Player>(this->GetOwningPlayer()->GetPawn());
				if (Owner)
				{
					Owner->InventoryComp->RemoveFromInventory(SelectedSlot->SlotInfo.ItemInfo, 1);
				}

				SelectedSlot->SlotInfo.Quantity--;
				if (SelectedSlot->SlotInfo.Quantity <= 0)
				{
					WB_SlotList->RemoveChildAt(i);
					Img_SelectedItem->SetBrushFromTexture(nullptr);
					TB_ItemName->SetText(FText::FromString(TEXT("")));
					TB_Price->SetText(FText::FromString(TEXT("")));
					TB_Quantity->SetText(FText::FromString(TEXT("")));
				}
				else
				{
					DIsplaySelectedItemInfo();
				}
				break;
			}

		}
	}
}

void UInventoryWidget::UseItem()
{
	if (SelectedSlot)
	{
		auto* Temp = GetWorld()->SpawnActor<AHG_ItemBase>(SelectedSlot->slot);
		ThrowAwaySelectedItem();
	}
}
