// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_ItemPurchaseWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "HG_Player.h"
#include "HG_PlayerInventoryComponent.h"
#include "HG_PlayerGoodsComponent.h"
#include "HG_GameInstance.h"

void UHG_ItemPurchaseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!Btn_Purchase->OnClicked.IsBound())
	{
		Btn_Purchase->OnClicked.AddDynamic(this, &UHG_ItemPurchaseWidget::OnClickPurchaseButton);
	}
	if (!Btn_Exit->OnClicked.IsBound())
	{
		Btn_Exit->OnClicked.AddDynamic(this, &UHG_ItemPurchaseWidget::OnClickExitButton);
	}

	GI = Cast<UHG_GameInstance>(GetWorld()->GetGameInstance());
}

void UHG_ItemPurchaseWidget::OnClickPurchaseButton()
{
	if (GI->IsValidItem(LookingItemData.ItemName))
	{

		auto* pc = Cast<APlayerController>(Owner->Controller);
		if (pc)
		{
			pc->SetShowMouseCursor(false);
			Owner->bCanMove = true;
			Owner->bToggle = !Owner->bToggle;
		}
		if (Owner->GoodsComp->GetGold() <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Gold = 0"));
			RemoveFromParent();
			return;
		}
		else
		{
			Owner->GoodsComp->SubGold(LookingItemData.ItemPrice);
			UE_LOG(LogTemp, Warning, TEXT("%d"), Owner->GoodsComp->GetGold());
			Owner->InventoryComp->AddtoInventory(LookingItemData, 1);
			RemoveFromParent();
		}
	}
}

void UHG_ItemPurchaseWidget::OnClickExitButton()
{
	RemoveFromParent();
	auto* pc = Cast<APlayerController>(Owner->Controller);
	if (pc)
	{
		pc->SetShowMouseCursor(false);
		Owner->bCanMove = true;
		Owner->bToggle = !Owner->bToggle;
	}
}

void UHG_ItemPurchaseWidget::SetItemInfo(FItemData ItemInfo)
{
	LookingItemData = ItemInfo;
	Img_ItemImg->SetBrushFromTexture(ItemInfo.ItemIcon);
	TB_ItemName->SetText(FText::FromString(ItemInfo.ItemName));
	TB_ItemPrice->SetText(FText::FromString(FString::Printf(TEXT("%d Point"), ItemInfo.ItemPrice)));
}

void UHG_ItemPurchaseWidget::SetOwner(AHG_Player* Value)
{
	Owner = Value;
}
