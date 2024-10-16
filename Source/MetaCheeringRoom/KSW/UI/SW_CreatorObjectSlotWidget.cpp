// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_CreatorObjectSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "../CreatorStorageSubsystem.h"

void USW_CreatorObjectSlotWidget::NativeConstruct()
{

}

void USW_CreatorObjectSlotWidget::SetObject(FCreatorObjectData* data)
{
		// ObjectData = data;
	ObjectName->SetText(FText::FromString(data->ItemName));
	ObjectImage->SetBrushFromTexture(data->ItemIcon);
}

void USW_CreatorObjectSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InPointerEvent, UDragDropOperation*& OutOperation)
{

}

FReply USW_CreatorObjectSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

bool USW_CreatorObjectSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return true;
}
