// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "HG_Player.h"
#include "HG_PlayerInventoryComponent.h"
#include "SlotStruct.h"
#include "HG_SlotWidget.h"
#include "Components/WrapBox.h"

void UInventoryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super:: NativeTick(MyGeometry,InDeltaTime);

	InitInventoryUI();
}

void UInventoryWidget::InitInventoryUI()
{
	auto* OwningPlayer = Cast<AHG_Player>(this->GetOwningPlayerPawn());
	if (OwningPlayer)
	{
		for (auto slot : OwningPlayer->InventoryComp->Inventory)
		{	
			UHG_SlotWidget* SlotWidget = CreateWidget<UHG_SlotWidget>(this->GetOwningPlayerPawn(),SlotWidgetFactory);
			if(SlotWidget)
			{
				WB_SlotList->AddChildToWrapBox(SlotWidget);
			}
		}
	}
}
