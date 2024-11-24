// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_CreatorHierarchyWidget.h"
#include "../CreatorMapSubsystem.h"
#include "SW_CreatorObjectSlotWidget.h"
#include "SW_CreatorHierarchyItemWidget.h"
#include "Components/ScrollBox.h"
#include "../CreatorObject/SW_CreatorObject.h"
#include "Components/Widget.h"
#include "SW_HierarchyDragOperation.h"
#include "Components/Button.h"
#include "../Creator/CreatorObjectRootActor.h"


void USW_CreatorHierarchyWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void USW_CreatorHierarchyWidget::ReloadItem()
{
	// 모든 아이템 삭제
	// ObjectScrollBox 자식 모두 제거
	for (int i = 0; i < SlotWidgets.Num(); i++)
	{
		SlotWidgets[i]->RemoveFromParent();
	}

	SlotWidgets.Empty();

	// 모든 아이템 다시 생성
	UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
	//const FCreatorMap& CMap = system->GetCreatorMap();

	//ReloadWidget(CMap.Objects, 0);
	ACreatorObjectRootActor* RootActor = system->GetRootActor();
	if (RootActor == nullptr)
		return;
	TArray<ASW_CreatorObject*> InCreatorObjects;
	TArray<AActor*> ChildActors;
	RootActor->GetAttachedActors(ChildActors);

	// RootActor의 자식들을 InCreatorObjects에 넣어준다.
	for (int32 i = 0; i < ChildActors.Num(); i++)
	{
		ASW_CreatorObject* CreatorObject = Cast<ASW_CreatorObject>(ChildActors[i]);
		if (CreatorObject)
		{
			InCreatorObjects.Add(CreatorObject);
		}
	}

	ReloadWidget(InCreatorObjects, 0);
}

bool USW_CreatorHierarchyWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	USW_HierarchyDragOperation* dragOperation = Cast<USW_HierarchyDragOperation>(InOperation);
	if (dragOperation)
	{
		if (dragOperation->bSameObject)
			return true;

		ASW_CreatorPlayerController* PC = Cast<ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());
		if (PC)
		{
			PC->DetachHirearchyObject(dragOperation->CreatorObject);
		}
	}

	return true;
}

void USW_CreatorHierarchyWidget::ReloadWidget(const TArray<ASW_CreatorObject*>& InCreatorObjects, int32 depth)
{
	for (int i = 0; i < InCreatorObjects.Num(); i++)
	{
		if (InCreatorObjects[i] == nullptr)
			continue;

		USW_CreatorHierarchyItemWidget* ChildWidget = CreateWidget<USW_CreatorHierarchyItemWidget>(GetWorld(), SlotFactory);
		ChildWidget->SetItem(InCreatorObjects[i], depth);
		ObjectScrollBox->AddChild(ChildWidget);

		SlotWidgets.Add(ChildWidget);
		TArray<AActor*> ChildActors;
		InCreatorObjects[i]->GetAttachedActors(ChildActors);
		TArray<ASW_CreatorObject*> ChildCreatorObject;
		int32 ChildCount = 0;
		for (int32 j = 0; j < ChildActors.Num(); j++)
		{
			ASW_CreatorObject* CreatorObject = Cast<ASW_CreatorObject>(ChildActors[j]);
			if (CreatorObject)
			{
				ChildCount++;
				ChildCreatorObject.Add(CreatorObject);
			}
		}

		if (ChildCount > 0)
		{
			ChildWidget->ExpandedButton->SetVisibility(ESlateVisibility::Visible);
			if (InCreatorObjects[i]->IsExpandedHierarchy)
			{
				
				ReloadWidget(ChildCreatorObject, depth + 1);
				ChildWidget->ExpandedButton->SetStyle(ChildWidget->ClickedExpandedButtonStyle);
			}
			else
			{
				ChildWidget->ExpandedButton->SetStyle(ChildWidget->DefaultExpandedButtonStyle);
			}
		}
		else
		{
			ChildWidget->ExpandedButton->SetVisibility(ESlateVisibility::Hidden);
		}

	}
}
