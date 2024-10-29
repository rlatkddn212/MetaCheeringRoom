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
#include "HG_EquipItem.h"
#include "HG_GameInstance.h"
#include "Net/UnrealNetwork.h"
#include "HG_PlayerAnimInstance.h"


void UInventoryWidget::NativeConstruct()
{
	if (!Btn_ThrowAway->OnClicked.IsBound())
	{
		Btn_ThrowAway->OnClicked.AddDynamic(this, &UInventoryWidget::ThrowAwaySelectedItem);
	}
	if (!Btn_Use->OnClicked.IsBound())
	{
		Btn_Use->OnClicked.AddDynamic(this, &UInventoryWidget::UseItem);
	}
	if (!Btn_CostumeCategory->OnClicked.IsBound())
	{
		Btn_CostumeCategory->OnClicked.AddDynamic(this, &UInventoryWidget::SelectCategory_Costume);
	}
	if (!Btn_ActiveCategory->OnClicked.IsBound())
	{
		Btn_ActiveCategory->OnClicked.AddDynamic(this, &UInventoryWidget::SelectCategory_Active);
	}
	if (!Btn_EmotionCategory->OnClicked.IsBound())
	{
		Btn_EmotionCategory->OnClicked.AddDynamic(this, &UInventoryWidget::SelectCategory_Emotion);
	}	
	if (!Btn_EmojiCategory->OnClicked.IsBound())
	{
		Btn_EmojiCategory->OnClicked.AddDynamic(this, &UInventoryWidget::SelectCategory_Emoji);
	}
	if (!Btn_SoundCategory->OnClicked.IsBound())
	{
		Btn_SoundCategory->OnClicked.AddDynamic(this, &UInventoryWidget::SelectCategory_Sound);
	}

	GI = Cast<UHG_GameInstance>(GetWorld()->GetGameInstance());
	SelectedCategory = WB_SlotList_Active;
}

void UInventoryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventoryWidget::InitInventoryUI()
{
	UE_LOG(LogTemp, Warning, TEXT("EquipList : %d"), EquipList.Num());
	WB_SlotList_Active->ClearChildren();
	WB_SlotList_Costume->ClearChildren();
	WB_SlotList_Emotion->ClearChildren();
	WB_SlotList_Emoji->ClearChildren();
	WB_SlotList_Sound->ClearChildren();
	SelectedSlot = nullptr;
	DIsplaySelectedItemInfo();
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
						WB_SlotList_Active->AddChildToWrapBox(SlotWidget);
					}
					else if (slot.ItemInfo.ItemCategory == EItemCategory::Category_Emotion)
					{
						WB_SlotList_Emotion->AddChildToWrapBox(SlotWidget);
					}
					else if (slot.ItemInfo.ItemCategory == EItemCategory::Category_Emoji)
					{
						WB_SlotList_Emoji->AddChildToWrapBox(SlotWidget);
					}
					else if (slot.ItemInfo.ItemCategory == EItemCategory::Category_Sound)
					{
						WB_SlotList_Sound->AddChildToWrapBox(SlotWidget);
					}
					else
					{
						WB_SlotList_Costume->AddChildToWrapBox(SlotWidget);
						CheckEquipitem();
 						for (auto EquipSlot : EquipList)
 						{
 							if (EquipSlot->SlotInfo.ItemInfo.ItemName == SlotWidget->SlotInfo.ItemInfo.ItemName)
 							{
 								SlotWidget->Img_Equip->SetVisibility(ESlateVisibility::HitTestInvisible);
 								EquipList[EquipList.Find(EquipSlot)] = SlotWidget;
 							}
 						}
					}
				}
			}
		}
	}
	SelectedCategory = WB_SlotList_Active;
	WS_Category->SetActiveWidgetIndex(0);
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
	TB_Use->SetText(FText::FromString(TEXT("사용하기")));
}

void UInventoryWidget::SelectCategory_Costume()
{
	SelectedCategory = WB_SlotList_Costume;
	WS_Category->SetActiveWidgetIndex(1);

	if (SelectedSlot != nullptr && EquipList.Contains(SelectedSlot))
	{
		TB_Use->SetText(FText::FromString(TEXT("해제하기")));
	}
	else
	{
		TB_Use->SetText(FText::FromString(TEXT("장착하기")));
	}
}

void UInventoryWidget::SelectCategory_Emotion()
{
	SelectedCategory = WB_SlotList_Emotion;
	WS_Category->SetActiveWidgetIndex(2);
	TB_Use->SetText(FText::FromString(TEXT("사용하기")));
}

void UInventoryWidget::SelectCategory_Emoji()
{
	SelectedCategory = WB_SlotList_Emoji;
	WS_Category->SetActiveWidgetIndex(3);
	TB_Use->SetText(FText::FromString(TEXT("사용하기")));
}

void UInventoryWidget::SelectCategory_Sound()
{
	SelectedCategory = WB_SlotList_Sound;
	WS_Category->SetActiveWidgetIndex(4);
	TB_Use->SetText(FText::FromString(TEXT("사용하기")));
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
		auto* Owner = Cast<AHG_Player>(this->GetOwningPlayer()->GetPawn());
		if (EquipList.Contains(SelectedSlot))
		{
			TB_Use->SetText(FText::FromString(TEXT("장착하기")));
			SelectedSlot->Img_Equip->SetVisibility(ESlateVisibility::Hidden);
			EquipList.Remove(SelectedSlot);
			Owner->UnequipItemToSocket(SelectedSlot->SlotInfo.ItemInfo.ItemName);
		}
		int32 ChildCount = SelectedCategory->GetChildrenCount();
		for (int32 i = 0; i < ChildCount; i++)
		{
			UHG_SlotWidget* Child = Cast<UHG_SlotWidget>(SelectedCategory->GetChildAt(i));
			if (Child == SelectedSlot)
			{
				if (Owner)
				{
					Owner->InventoryComp->RemoveFromInventory(SelectedSlot->SlotInfo.ItemInfo, 1);
				}
				SelectedSlot->SlotInfo.Quantity--;

				if (SelectedSlot->SlotInfo.Quantity == 0)
				{
					SelectedCategory->RemoveChildAt(i);
					SelectedSlot = nullptr;
				}
					DIsplaySelectedItemInfo();
				
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
			if (nullptr != OwningPlayer)
			{
				if (SelectedCategory == WB_SlotList_Active)
				{
					OwningPlayer->SpawnItem(SelectedSlot->SlotInfo.ItemInfo);
					ThrowAwaySelectedItem();
				}
				else if (SelectedCategory == WB_SlotList_Emoji || SelectedCategory == WB_SlotList_Sound)
				{
					OwningPlayer->SpawnItem(SelectedSlot->SlotInfo.ItemInfo);
				}
				else if (SelectedCategory == WB_SlotList_Costume)
				{
					if (GI->EquipSlotIndexList.Contains(SelectedSlot->MyIndex))
					{
						TB_Use->SetText(FText::FromString(TEXT("장착하기")));
						SelectedSlot->Img_Equip->SetVisibility(ESlateVisibility::Hidden);
						EquipList.Remove(SelectedSlot);
						GI->EquipSlotIndexList.Remove(GetSlotIndexInWB(SelectedSlot));
						OwningPlayer->UnequipItemToSocket(SelectedSlot->SlotInfo.ItemInfo.ItemName);
					}
					else
					{
						TB_Use->SetText(FText::FromString(TEXT("해제하기")));
						SelectedSlot->Img_Equip->SetVisibility(ESlateVisibility::HitTestInvisible);
						EquipList.Add(SelectedSlot);
						GI->EquipSlotIndexList.Add(GetSlotIndexInWB(SelectedSlot));
						OwningPlayer->EquipItemToSocket(SelectedSlot->SlotInfo.ItemInfo);
					}
				}
				else if(SelectedCategory == WB_SlotList_Emotion)
				{
					OwningPlayer->ServerRPC_Emotion(SelectedSlot->SlotInfo.ItemInfo.Montage);
					RemoveFromParent();
				}
			}
		}
	}
}

int32 UInventoryWidget::GetSlotIndexInWB(UWidget* SlotWidget)
{
	if (!WB_SlotList_Costume || !SlotWidget) return -1;

	const TArray<UWidget*>& Slots = WB_SlotList_Costume->GetAllChildren();

	int32 Index = Slots.IndexOfByKey(SlotWidget);

	return Index;
}

void UInventoryWidget::CheckEquipitem()
{
	if (GI->EquipSlotIndexList.Num() <= 0) return;

	for (auto n : GI->EquipSlotIndexList)
	{
		UHG_SlotWidget* EquipSlot = Cast<UHG_SlotWidget>(WB_SlotList_Costume->GetChildAt(n));
		if (EquipSlot)
		{
			EquipSlot->Img_Equip->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}
