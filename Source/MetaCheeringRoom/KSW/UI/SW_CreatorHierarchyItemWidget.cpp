// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_CreatorHierarchyItemWidget.h"
#include "Components/TextBlock.h"
#include "../CreatorMapSubsystem.h"
#include "Components/Spacer.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "SW_DragHierarchyItemWidget.h"
#include "SW_HierarchyDragOperation.h"
#include "Components/Image.h"
#include "Math/MathFwd.h"
#include "../SW_CreatorPlayerController.h"
#include "Components/Button.h"
#include "../CreatorObject/SW_CreatorObject.h"
#include "Delegates/Delegate.h"

void USW_CreatorHierarchyItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PC = Cast<ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());
	ExpandedButton->SetClickMethod(EButtonClickMethod::PreciseClick);
	ExpandedButton->OnClicked.AddDynamic(this, &USW_CreatorHierarchyItemWidget::OnClickExpandedButton);
}

void USW_CreatorHierarchyItemWidget::SetItem(ASW_CreatorObject* CreatorObject, int32 depth)
{
	// depth에 따라 크기를 늘린다.
	HierarchySpacer->SetSize(FVector2D(20 * depth, 1));

	// Set the item
	HierarchyCreatorObject = CreatorObject;

	if (PC == nullptr)
	{
		PC = Cast<ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());
	}

	OnHierarchyItemClicked.BindDynamic(PC, &ASW_CreatorPlayerController::DoSelectObject);
	HierarchyCreatorObject->OnChangeSelected.BindDynamic(this, &USW_CreatorHierarchyItemWidget::OnSelected);
	
	if (PC->SelectedObject == HierarchyCreatorObject)
	{
		HierarchyItemButton->SetStyle(ClickedButtonStyle);
	}
	else
	{
		HierarchyItemButton->SetStyle(DefaultButtonStyle);
	}

	// InCreatorObject->ObjectName;
	FString NameStr = HierarchyCreatorObject->GetCreatorObjectName();
	if (NameStr.Len() > 20)
	{
		NameStr = NameStr.Left(20);
	}

	NameText->SetText(FText::FromString(NameStr));
}

void USW_CreatorHierarchyItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InPointerEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InPointerEvent, OutOperation);
	USW_DragHierarchyItemWidget* widget = CreateWidget<USW_DragHierarchyItemWidget>(GetWorld(), DragFactory);
	USW_HierarchyDragOperation* dragOperation = NewObject<USW_HierarchyDragOperation>();

	widget->HierarchyItemImage->SetBrush(IconImage->Brush);
	dragOperation->DefaultDragVisual = widget;
	dragOperation->ItemName = HierarchyCreatorObject->GetName();
	dragOperation->CreatorObject = HierarchyCreatorObject;
	
	OutOperation = dragOperation;
}

FReply USW_CreatorHierarchyItemWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
	FEventReply ret = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	HierarchyItemButton->SetStyle(ClickedButtonStyle);
	// 아이템 셀렉트를 바꾼다.
	//PC->DoSelectObject(CreatorObject);
	if (OnHierarchyItemClicked.IsBound())
		OnHierarchyItemClicked.Execute(HierarchyCreatorObject);

	bIsDragging = true;
	return ret.NativeReply;
}

void USW_CreatorHierarchyItemWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	if (!bIsDragging)
		HierarchyItemButton->SetStyle(DefaultButtonStyle);
	bIsDragging = false;
}

void USW_CreatorHierarchyItemWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

	// 호버스타일로 변경한다.
	if (!bIsDragging)
		HierarchyItemButton->SetStyle(HoveredButtonStyle);

	// 자기 자신인지 검사
	USW_HierarchyDragOperation* dragOperation = Cast<USW_HierarchyDragOperation>(InOperation);
	if (dragOperation)
	{
		if (dragOperation->CreatorObject == HierarchyCreatorObject)
		{
			dragOperation->bSameObject = true;
		}	
	}
}

void USW_CreatorHierarchyItemWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	if (!bIsDragging)
		HierarchyItemButton->SetStyle(DefaultButtonStyle);

	// 자기 자신인지 검사
	USW_HierarchyDragOperation* dragOperation = Cast<USW_HierarchyDragOperation>(InOperation);
	if (dragOperation)
	{
		if (dragOperation->CreatorObject == HierarchyCreatorObject)
		{
			dragOperation->bSameObject = false;
		}
	}
}

bool USW_CreatorHierarchyItemWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	USW_HierarchyDragOperation* dragOperation = Cast<USW_HierarchyDragOperation>(InOperation);
	if (!bIsDragging)
		HierarchyItemButton->SetStyle(DefaultButtonStyle);

	bIsDragging = false;
	if (dragOperation)
	{
		UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
		if (system->IsChildObject(dragOperation->CreatorObject, HierarchyCreatorObject))
		{
			return false;
		}

		PC->AttachHirearchyObject(HierarchyCreatorObject, dragOperation->CreatorObject);
	}

	return true;
}

void USW_CreatorHierarchyItemWidget::OnSelected(bool isSelected)
{
	if (isSelected)
	{
		HierarchyItemButton->SetStyle(ClickedButtonStyle);
	}
	else
	{
		HierarchyItemButton->SetStyle(DefaultButtonStyle);
	}
}

void USW_CreatorHierarchyItemWidget::OnClickExpandedButton()
{
	// 아이템을 펼친다.
	if (HierarchyCreatorObject)
	{
		HierarchyCreatorObject->IsExpandedHierarchy = !HierarchyCreatorObject->IsExpandedHierarchy;
		if (PC)
		{
			PC->ReloadHierarchy();
		}
	}
	
}
