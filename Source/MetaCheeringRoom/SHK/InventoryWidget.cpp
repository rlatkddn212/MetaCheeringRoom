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
#include "Animation/WidgetAnimation.h"
#include "MovieScene.h"


void UInventoryWidget::NativeConstruct()
{
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
}

void UInventoryWidget::InitInventoryUI()
{
	// 모든 WrapBox의 자식들을 없애준다. (해주지 않으면 아이템이 복사됨)
	WB_SlotList_Active->ClearChildren();
	WB_SlotList_Costume->ClearChildren();
	WB_SlotList_Emotion->ClearChildren();
	WB_SlotList_Emoji->ClearChildren();
	WB_SlotList_Sound->ClearChildren();
	// 선택 슬롯도 초기화 (UX 적으로 인벤토리를 껐다 켰을 때 선택된 슬롯도 없는 것이 맞다고 판단)
	SelectedSlot = nullptr;
	// 현재 이 인벤토리 위젯이 어떤 Pawn 에게 소유 되었는지 검사
	if (this->GetOwningPlayer() != nullptr)
	{
		// 소유하고 있는 Pawn 이 있다면
		// 해당 Pawn 을 Player 로 Cast 한다.
		auto* OwningPlayer = Cast<AHG_Player>(this->GetOwningPlayer()->GetPawn());
		if (OwningPlayer)
		{
			// Cast 에 성공했다면
			
			// 마지막으로 선택했던 카테고리를 표시
			WS_Category->SetActiveWidgetIndex(memory);
			
			// 해당 Player 가 가진 Inventory 의 모든 요소를 접근 
			for (auto slot : OwningPlayer->InventoryComp->Inventory)
			{
				// 각 슬롯(요소)별로 위젯을 만듦
				UHG_SlotWidget* SlotWidget = CreateWidget<UHG_SlotWidget>(this, SlotWidgetFactory);
				if (SlotWidget)
				{
					// 만드는데 성공했다면
					
					// 슬롯 위젯을 현재 접근 중인 인벤토리 슬롯의 데이터로 초기화해준다.
					SlotWidget->InitSlot(slot);
					SlotWidget->SetItemIcon();

					// 슬롯 위젯의 주인을 설정해준다. (인벤토리 UI에서 상대적인 자기 자신에 대한 정보를 알아야하기 때문)
					SlotWidget->SetOwner(this);

					// 현재 접근하고 있는 슬롯의 카테고리에 맞는 WrapBox에 넣어주기 위한 EItemCategory 검사
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
							// 갖고있는 ItemData의 ItemName으로 현재 생성한 SlotWidget가 EquipList에 요소로 존재했었는지 검사
 							if (EquipSlot->SlotInfo.ItemInfo.ItemName == SlotWidget->SlotInfo.ItemInfo.ItemName)
 							{
								// EquipList에 존재했었다면
								
								// EquipList에서 기존에 장착했던 아이템을 다시 생성한 SlotWidget로 갱신해줌
 								EquipList[EquipList.Find(EquipSlot)] = SlotWidget;
 							}
 						}
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

// Active 카테고리 버튼이 클릭 됐을 때 실행되는 함수
void UInventoryWidget::SelectCategory_Active()
{
	SelectedCategory = WB_SlotList_Active;
	WS_Category->SetActiveWidgetIndex(0);

	memory = 0;

	CheckButtonClick(Btn_ActiveCategory);
}

// Costume 카테고리 버튼이 클릭 됐을 때 실행되는 함수
void UInventoryWidget::SelectCategory_Costume()
{
	SelectedCategory = WB_SlotList_Costume;
	WS_Category->SetActiveWidgetIndex(1);

	memory = 1;

	CheckButtonClick(Btn_CostumeCategory);
}

// Emotion 카테고리 버튼이 클릭 됐을 때 실행되는 함수
void UInventoryWidget::SelectCategory_Emotion()
{
	SelectedCategory = WB_SlotList_Emotion;
	WS_Category->SetActiveWidgetIndex(2);

	memory = 2;

	CheckButtonClick(Btn_EmotionCategory);
}

// Emoji 카테고리 버튼이 클릭 됐을 때 실행되는 함수
void UInventoryWidget::SelectCategory_Emoji()
{
	SelectedCategory = WB_SlotList_Emoji;
	WS_Category->SetActiveWidgetIndex(3);

	memory = 3;

	CheckButtonClick(Btn_EmojiCategory);
}

// Sound 카테고리 버튼이 클릭 됐을 때 실행되는 함수
void UInventoryWidget::SelectCategory_Sound()
{
	SelectedCategory = WB_SlotList_Sound;
	WS_Category->SetActiveWidgetIndex(4);

	memory = 4;

	CheckButtonClick(Btn_SoundCategory);
}

// 아이템 버리기
void UInventoryWidget::ThrowAwaySelectedItem()
{
	if (SelectedSlot)
	{
		// 위젯의 주인 찾기  
		auto* Owner = Cast<AHG_Player>(this->GetOwningPlayer()->GetPawn());

		// 선택된 슬롯이 장착된 아이템 슬롯 리스트에 있는지 확인
		if (EquipList.Contains(SelectedSlot))
		{			
			// 장착된 아이템 슬롯 리스트에서 선택된 슬롯 제거
			EquipList.Remove(SelectedSlot);
			
			// 인벤토리 위젯의 주인이 장착하고 있는 아이템을 장착 해제
			Owner->UnequipItemToSocket(SelectedSlot->SlotInfo.ItemInfo.ItemName);
		}
		// 현재 선택된 Category WrapBox의 자식 개수를 가져옴
		int32 ChildCount = SelectedCategory->GetChildrenCount();
		// 자식의 수만큼 반복함
		for (int32 i = 0; i < ChildCount; i++)
		{
			// SelectedCategory 에서 i 번째 자식을  가져와서 UHG_SlotWidget 으로 Casting 
			UHG_SlotWidget* Child = Cast<UHG_SlotWidget>(SelectedCategory->GetChildAt(i));

			// 선택한 슬롯과 SelectedCategory의 i 번째 자식이 같은지 확인
			if (Child == SelectedSlot)
			{
				// 같다면

				// NullCheck
				if (Owner)
				{
					// Owner의 인벤토리에서 선택된 슬롯의 데이터와 같은 아이템을 제거
					Owner->InventoryComp->RemoveFromInventory(SelectedSlot->SlotInfo.ItemInfo, 1);
				}
				// 슬롯의 Quantity를 하나 줄임
				SelectedSlot->SlotInfo.Quantity--;

				// 슬롯의 Quantity가 0이 되면
				if (SelectedSlot->SlotInfo.Quantity == 0)
				{
					// 선택된 카테고리의 i번째 자식을 없애고
					SelectedCategory->RemoveChildAt(i);
					// SelectedSlot을 비워줌
					SelectedSlot = nullptr;
				}
				break;
			}

		}
	}
}

// 아이템 사용하기
void UInventoryWidget::UseItem()
{
	// NullCheck
	if (SelectedSlot)
	{
		// NullCheck
		if (this->GetOwningPlayer() != nullptr)
		{
			// 위젯의 주인 찾기  
			auto* OwningPlayer = Cast<AHG_Player>(this->GetOwningPlayer()->GetPawn());
			// NullCheck
			if (nullptr != OwningPlayer)
			{
				// 선택된 슬롯이 무엇인지 확인
				if (SelectedCategory == WB_SlotList_Active)
				{
					// Active에서 아이템을 사용했다면
					
					// 아이템을 월드에 스폰 시키고 사용하는 AHG_Player의 SpawnItem 함수 사용
					OwningPlayer->SpawnItem(SelectedSlot->SlotInfo.ItemInfo);

					// 아이템 1개 감소
					ThrowAwaySelectedItem();
				}
				else if (SelectedCategory == WB_SlotList_Emoji || SelectedCategory == WB_SlotList_Sound)
				{
					// 이모티콘 혹은 효과음 아이템을 사용했다면

					// 아이템을 월드에 스폰 시키고 사용하는 AHG_Player의 SpawnItem 함수 사용
					OwningPlayer->SpawnItem(SelectedSlot->SlotInfo.ItemInfo);
				}
				else if (SelectedCategory == WB_SlotList_Costume)
				{
					// GameInstance의 EquipSlotIndexList에 내가 선택한 슬롯의 아이템의 인덱스가 포함되어 있는지 확인 ==> 장착한 아이템인지 확인
					if (GI->EquipSlotIndexList.Contains(SelectedSlot->MyIndex))
					{

						// EquipList에서 선택한 슬롯을 제거
						EquipList.Remove(SelectedSlot);

						// GameInstance의 EquipSlotIndexList에서 현재 슬롯의 인덱스 제거
						GI->EquipSlotIndexList.Remove(GetSlotIndexInWB(SelectedSlot));

						// 플레이어 아이템 장착 해제
						OwningPlayer->UnequipItemToSocket(SelectedSlot->SlotInfo.ItemInfo.ItemName);
					}
					else
					{
						// 장착하지 않았다면 

						// EquipList에서 선택한 슬롯을 추가
						EquipList.Add(SelectedSlot);

						// GameInstance의 EquipSlotIndexList에서 현재 슬롯의 인덱스 추가
						GI->EquipSlotIndexList.Add(GetSlotIndexInWB(SelectedSlot));

						// 플레이어 아이템 장착
						OwningPlayer->EquipItemToSocket(SelectedSlot->SlotInfo.ItemInfo);
					}
				}
				else if(SelectedCategory == WB_SlotList_Emotion)
				{
					// 이모션 아이템을 사용했다면

					// 플레이어가 Montage를 실행하도록 함
					if (OwningPlayer->Gender == 1)
					{
						OwningPlayer->ServerRPC_Emotion(SelectedSlot->SlotInfo.ItemInfo.Woman_Montage);
					}
					else if (OwningPlayer->Gender == 2)
					{
						OwningPlayer->ServerRPC_Emotion(SelectedSlot->SlotInfo.ItemInfo.Man_Montage);
					}
				}
			}
		}
	}
}

// 현재 선택한 슬롯이 부모에서 몇 번째 자식인지 반환받는 함수
int32 UInventoryWidget::GetSlotIndexInWB(UWidget* SlotWidget)
{
	if (!WB_SlotList_Costume || !SlotWidget) return -1;

	const TArray<UWidget*>& Slots = WB_SlotList_Costume->GetAllChildren();

	int32 Index = Slots.IndexOfByKey(SlotWidget);

	return Index;
}

// 아이템이 장착되었는지 확인하는 함수
void UInventoryWidget::CheckEquipitem()
{
	if (GI->EquipSlotIndexList.Num() <= 0) return;

	for (auto n : GI->EquipSlotIndexList)
	{
		UHG_SlotWidget* EquipSlot = Cast<UHG_SlotWidget>(WB_SlotList_Costume->GetChildAt(n));
		if (EquipSlot)
		{
		//	EquipSlot->Img_Equip->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}

void UInventoryWidget::CheckButtonClick(UButton* p_Button)
{
	Btn_SoundCategory->SetBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 0.5f));
	Btn_ActiveCategory->SetBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 0.5f));
	Btn_CostumeCategory->SetBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 0.5f));
	Btn_EmotionCategory->SetBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 0.5f));
	Btn_EmojiCategory->SetBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 0.5f));

	p_Button->SetBackgroundColor(FLinearColor(0.0f,0.26f,0.15f,0.5f));
}

void UInventoryWidget::PlayAppearAnimation(bool Play_Forward)
{
	if (IsAnimationPlaying(Appear))
	{
		StopAnimation(Appear);
		StopAnimation(Disappear);
	}

	if(IsAnimationPlaying(Disappear))
	{
		StopAnimation(Disappear);
	}

	if(!Appear || !Disappear) return;

	if (Play_Forward)
	{
		PlayAnimation(Appear);
	}
	else
	{	
		PlayAnimation(Disappear);
	}
}

