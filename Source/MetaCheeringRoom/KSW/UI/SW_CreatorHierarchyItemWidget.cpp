// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_CreatorHierarchyItemWidget.h"
#include "Components/TextBlock.h"
#include "../CreatorMapSubsystem.h"
#include "Components/Spacer.h"

void USW_CreatorHierarchyItemWidget::SetItem(const TSharedPtr<struct FCreatorObject>& InCreatorObject, int32 depth)
{
	// depth�� ���� ũ�⸦ �ø���.
	HierarchySpacer->SetSize(FVector2D(20 * depth, 1));

	// Set the item
	CreatorObject = InCreatorObject;

	// InCreatorObject->ObjectName;
	NameText->SetText(FText::FromString(CreatorObject->ObjectName));
}
