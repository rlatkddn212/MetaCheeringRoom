// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_CreatorHierarchyWidget.h"
#include "../CreatorMapSubsystem.h"
#include "SW_CreatorObjectSlotWidget.h"
#include "SW_CreatorHierarchyItemWidget.h"
#include "Components/ScrollBox.h"
#include "../SW_CreatorObject.h"
#include "Components/Widget.h"
#include "SW_HierarchyDragOperation.h"


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
	const FCreatorMap& CMap = system->GetCreatorMap();

	ReloadWidget(CMap.Objects, 0);
}

bool USW_CreatorHierarchyWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	USW_HierarchyDragOperation* dragOperation = Cast<USW_HierarchyDragOperation>(InOperation);
	if (dragOperation)
	{
		if (dragOperation->bSameObject)
			return true;
		
		UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
		ASW_CreatorObject* PrevParent = system->FindParentObject(dragOperation->CreatorObject);
		system->DetechObject(PrevParent, dragOperation->CreatorObject);
		system->AttachObject(nullptr, dragOperation->CreatorObject);

		ASW_CreatorPlayerController* PC = Cast<ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());
		if (PC) 
			PC->ReloadHierarchy();
	}

	return true;
}

void USW_CreatorHierarchyWidget::ReloadWidget(const TArray<ASW_CreatorObject*>& InCreatorObjects, int32 depth)
{
	for (int i = 0; i < InCreatorObjects.Num(); i++)
	{
		USW_CreatorHierarchyItemWidget* ChildWidget = CreateWidget<USW_CreatorHierarchyItemWidget>(GetWorld(), SlotFactory);
		ChildWidget->SetItem(InCreatorObjects[i], depth);
		ObjectScrollBox->AddChild(ChildWidget);

		SlotWidgets.Add(ChildWidget);
		TArray<AActor*> ChildActors;
		InCreatorObjects[i]->GetAttachedActors(ChildActors);
		TArray<ASW_CreatorObject*> ChildCreatorObject;

		for (int j = 0; j < ChildActors.Num(); j++)
		{
			ASW_CreatorObject* CreatorObject = Cast<ASW_CreatorObject>(ChildActors[j]);
			if (CreatorObject)
			{
				ChildCreatorObject.Add(CreatorObject);
			}
		}

		ReloadWidget(ChildCreatorObject, depth + 1);
	}
}
