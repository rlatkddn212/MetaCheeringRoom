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

	Btn_Purchase->OnClicked.AddDynamic(this, &UHG_ItemPurchaseWidget::OnClickPurchaseButton);
	Btn_Exit->OnClicked.AddDynamic(this, &UHG_ItemPurchaseWidget::OnClickExitButton);

	GI = Cast<UHG_GameInstance>(GetWorld()->GetGameInstance());
}

void UHG_ItemPurchaseWidget::OnClickPurchaseButton()
{
	if (GI->IsValidItem(LookingItemData.ItemName))
	{
		if (0 <= Owner->GoodsComp->AddGold(-1 * LookingItemData.ItemPrice))
		{
			Owner->InventoryComp->AddtoInventory(LookingItemData, 1);
		}
		RemoveFromParent();
		auto* pc = Cast<APlayerController>(Owner->Controller);
		if (pc)
		{
			pc->SetShowMouseCursor(false);
			Owner->bCanMove = true;
			Owner->bToggle = !Owner->bToggle;
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
	UE_LOG(LogTemp,Warning,TEXT("UHG_ItemPurchaseWidget::SetItemInfo"));
	LookingItemData = ItemInfo;
	Img_ItemImg->SetBrushFromTexture(ItemInfo.ItemIcon);
	TB_ItemName->SetText(FText::FromString(ItemInfo.ItemName));
	TB_ItemPrice->SetText(FText::FromString(FString::Printf(TEXT("%dPoint"), ItemInfo.ItemPrice)));
}

void UHG_ItemPurchaseWidget::SetOwner(AHG_Player* Value)
{
	Owner = Value;
}
