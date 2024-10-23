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
#include "../SW_CreatorObject.h"

void USW_CreatorHierarchyItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PC = Cast<ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());
}

void USW_CreatorHierarchyItemWidget::SetItem(ASW_CreatorObject* CreatorObject, int32 depth)
{
	// depth�� ���� ũ�⸦ �ø���.
	HierarchySpacer->SetSize(FVector2D(20 * depth, 1));

	// Set the item
	HierarchyCreatorObject = CreatorObject;

	// InCreatorObject->ObjectName;
	NameText->SetText(FText::FromString(HierarchyCreatorObject->GetName()));
}


void USW_CreatorHierarchyItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InPointerEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InPointerEvent, OutOperation);
	USW_DragHierarchyItemWidget* widget = CreateWidget<USW_DragHierarchyItemWidget>(GetWorld(), DragFactory);
	USW_HierarchyDragOperation* dragOperation = NewObject<USW_HierarchyDragOperation>();

	widget->HierarchyItemImage->SetBrush(IconImage->Brush);
	dragOperation->DefaultDragVisual = widget;
	dragOperation->ItemName = HierarchyCreatorObject->GetName();
	
	OutOperation = dragOperation;
}

FReply USW_CreatorHierarchyItemWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
	FEventReply ret = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	HierarchyItemButton->SetStyle(ClickedButtonStyle);
	// ������ ����Ʈ�� �ٲ۴�.
	//PC->DoSelectObject(CreatorObject);

	return ret.NativeReply;
}

void USW_CreatorHierarchyItemWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	HierarchyItemButton->SetStyle(DefaultButtonStyle);
}

void USW_CreatorHierarchyItemWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

	// ȣ����Ÿ�Ϸ� �����Ѵ�.
	HierarchyItemButton->SetStyle(HoveredButtonStyle);

}

void USW_CreatorHierarchyItemWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	HierarchyItemButton->SetStyle(DefaultButtonStyle);
}

bool USW_CreatorHierarchyItemWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	USW_HierarchyDragOperation* dragOperation = Cast<USW_HierarchyDragOperation>(InOperation);
	HierarchyItemButton->SetStyle(DefaultButtonStyle);
	if (dragOperation)
	{
		// dragOperation->ItemName;
		// dragOperation->ItemIcon;
		// dragOperation->ItemData;
		UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
		const FCreatorMap& CMap = system->GetCreatorMap();

		ASW_CreatorObject* PrevParent = system->FindParentObject(dragOperation->CreatorObject);
		if (PrevParent)
		{
			system->RemoveChildObject(PrevParent, dragOperation->CreatorObject);
			system->AddChildObject(HierarchyCreatorObject, dragOperation->CreatorObject);
		}

		// PC���� ȭ���� ���÷�����Ų��.
	}

	return true;
}
