// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_CreatorObjectSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "../CreatorStorageSubsystem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "SW_ObjectDragOperation.h"
#include "SW_DragObjectItemWidget.h"
#include "../SW_CreatorPlayerController.h"

void USW_CreatorObjectSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Visible);
}

void USW_CreatorObjectSlotWidget::SetObject(FCreatorObjectData* data)
{
	ObjectData = data;
	// ObjectData = data;
	ObjectName->SetText(FText::FromString(data->ItemName));
	ObjectImage->SetBrushFromTexture(data->ItemIcon);
}

void USW_CreatorObjectSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InPointerEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InPointerEvent, OutOperation);
	// DragFactory widget 생성
	USW_DragObjectItemWidget* widget = CreateWidget<USW_DragObjectItemWidget>(GetWorld(), DragFactory);
	USW_ObjectDragOperation* dragOperation = NewObject<USW_ObjectDragOperation>();
	
	widget->ObjectImage->SetBrushFromTexture(ObjectData->ItemIcon);
	dragOperation->DefaultDragVisual = widget;
	dragOperation->ItemName = ObjectData->ItemName;

	// 플레이어 컨트롤
	ASW_CreatorPlayerController* controller = Cast<ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());
	if (controller)
	{
		controller->CreatingDummyObject(ObjectData);
	}

	OutOperation = dragOperation;
}

FReply USW_CreatorObjectSlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

	FEventReply ret = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);


	return ret.NativeReply;
}

void USW_CreatorObjectSlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// 플레이어 컨트롤
	ASW_CreatorPlayerController* controller = Cast<ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());
	if (controller)
	{
		controller->DeleteDummyObject();
	}

}
