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
#include "Components/WidgetSwitcher.h"

void UInventoryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventoryWidget::NativeConstruct()
{
	//Btn_Use->OnClicked.AddDynamic();
	Btn_ThrowAway->OnClicked.AddDynamic(this, &UInventoryWidget::ThrowAwaySelectedItem);
	Btn_Use->OnClicked.AddDynamic(this, &UInventoryWidget::UseItem);
	Btn_CostumeCategory->OnClicked.AddDynamic(this, &UInventoryWidget::SelectCategory_Costume);
	Btn_ActiveCategory->OnClicked.AddDynamic(this, &UInventoryWidget::SelectCategory_Active);

	SelectedCategory = WB_SlotList_Active;
}

void UInventoryWidget::InitInventoryUI()
{
	WB_SlotList_Active->ClearChildren();
	WB_SlotList_Costume->ClearChildren();
	SelectedSlot = nullptr;
	if (this->GetOwningPlayer() != nullptr)
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
					if (slot.ItemInfo.ItemCategory == EItemCategory::Category_Active)
					{
						UE_LOG(LogTemp, Warning, TEXT("Category_Active"));
						WB_SlotList_Active->AddChildToWrapBox(SlotWidget);
					}
					else if (slot.ItemInfo.ItemCategory == EItemCategory::Category_Costume)
					{

						UE_LOG(LogTemp, Warning, TEXT("Category_Costume"));
						WB_SlotList_Costume->AddChildToWrapBox(SlotWidget);
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Error"));
					}
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

void UInventoryWidget::SelectCategory_Active()
{
	SelectedCategory = WB_SlotList_Active;
	WS_Category->SetActiveWidgetIndex(0);
}

void UInventoryWidget::SelectCategory_Costume()
{
	SelectedCategory = WB_SlotList_Costume;
	WS_Category->SetActiveWidgetIndex(1);
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
		Img_SelectedItem->SetBrushFromMaterial(DefaultImage);
		TB_ItemName->SetText(FText::FromString(TEXT("")));
		TB_Price->SetText(FText::FromString(TEXT("")));
		TB_Quantity->SetText(FText::FromString(TEXT("")));
	}
}

void UInventoryWidget::ThrowAwaySelectedItem()
{
	if (SelectedSlot)
	{
		int32 ChildCount = SelectedCategory->GetChildrenCount();
		for (int32 i = 0; i < ChildCount; i++)
		{
			UHG_SlotWidget* Child = Cast<UHG_SlotWidget>(SelectedCategory->GetChildAt(i));
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
					SelectedCategory->RemoveChildAt(i);
					Img_SelectedItem->SetBrushFromMaterial(DefaultImage);
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
		if (this->GetOwningPlayer() != nullptr)
		{
			auto* OwningPlayer = Cast<AHG_Player>(this->GetOwningPlayer()->GetPawn());

			auto* SpawnedItem = GetWorld()->SpawnActor<AHG_ItemBase>(SelectedSlot->SlotInfo.ItemInfo.ItemClass, OwningPlayer->GetActorLocation(), OwningPlayer->GetActorRotation());
			SpawnedItem->SetOwner(OwningPlayer);
			if (SpawnedItem)
			{
				SpawnedItem->SetActorHiddenInGame(true);
				SpawnedItem->Use();
				ThrowAwaySelectedItem();
			}
		}
	}
}
