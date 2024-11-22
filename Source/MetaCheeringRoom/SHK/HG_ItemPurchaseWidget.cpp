// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_ItemPurchaseWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "HG_Player.h"
#include "HG_PlayerInventoryComponent.h"
#include "HG_PlayerGoodsComponent.h"
#include "HG_GameInstance.h"
#include "Kismet/GameplayStatics.h"

void UHG_ItemPurchaseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GI = Cast<UHG_GameInstance>(GetWorld()->GetGameInstance());
}

void UHG_ItemPurchaseWidget::OnPressQ()
{
	
	if (GI && GI->IsValidItem(LookingItemData.ItemName))
	{
		if (Owner->PC)
		{
			Owner->PC->SetShowMouseCursor(false);
			Owner->bCanMove = true;
			Owner->bToggle = !Owner->bToggle;
			Owner->PC->SetInputMode(FInputModeGameOnly());
			Owner->PurchaseWidget = nullptr;
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

			UGameplayStatics::PlaySound2D(GetWorld(), Owner->PurchaseSound);

			RemoveFromParent();
		}
	}
}

void UHG_ItemPurchaseWidget::OnClickEmptySpace()
{
	RemoveFromParent();
	if (Owner->PC)
	{
		Owner->PC->SetShowMouseCursor(false);
		Owner->bCanMove = true;
		Owner->bToggle = !Owner->bToggle;
		Owner->PC->SetInputMode(FInputModeGameOnly());
	}
}

void UHG_ItemPurchaseWidget::SetItemInfo(FItemData ItemInfo)
{
	LookingItemData = ItemInfo;
	TB_ItemName->SetText(FText::FromString(ItemInfo.ItemName));
	TB_ItemPrice->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemInfo.ItemPrice)));
}

void UHG_ItemPurchaseWidget::SetOwner(AHG_Player* Value)
{
	Owner = Value;
}
