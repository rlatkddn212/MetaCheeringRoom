// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_CreatorHierarchyWidget.h"
#include "../CreatorMapSubsystem.h"
#include "SW_CreatorObjectSlotWidget.h"
#include "SW_CreatorHierarchyItemWidget.h"
#include "Components/ScrollBox.h"


void USW_CreatorHierarchyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//UCreatorStorageSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorStorageSubsystem>();
	//TArray<FCreatorObjectData*> CreatorObjects = system->GetCreatorObjects();

	//for (int i = 0; i < CreatorObjects.Num(); i++)
	//{
	//	USW_CreatorObjectSlotWidget* ChildWidget = CreateWidget<USW_CreatorObjectSlotWidget>(GetWorld(), SlotFactory);
	//	ChildWidget->SetObject(CreatorObjects[i]);
	//	ObjectScrollBox->AddChild(ChildWidget);
	//}
}

void USW_CreatorHierarchyWidget::ReloadItem()
{
	// ��� ������ ����
	// ObjectScrollBox �ڽ� ��� ����
	for (int i = 0; i < SlotWidgets.Num(); i++)
	{
		SlotWidgets[i]->RemoveFromParent();
	}

	SlotWidgets.Empty();

	// ��� ������ �ٽ� ����
	UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
	const FCreatorMap& CMap = system->GetCreatorMap();

	ReloadWidget(CMap.Objects, 0);
}

void USW_CreatorHierarchyWidget::ReloadWidget(const TArray<TSharedPtr<FCreatorObject>>& InCreatorObjects, int32 depth)
{
	for (int i = 0; i < InCreatorObjects.Num(); i++)
	{
		USW_CreatorHierarchyItemWidget* ChildWidget = CreateWidget<USW_CreatorHierarchyItemWidget>(GetWorld(), SlotFactory);
		ChildWidget->SetItem(InCreatorObjects[i], depth);
		ObjectScrollBox->AddChild(ChildWidget);

		SlotWidgets.Add(ChildWidget);

		ReloadWidget(InCreatorObjects[i]->Objects, depth + 1);
	}
}
