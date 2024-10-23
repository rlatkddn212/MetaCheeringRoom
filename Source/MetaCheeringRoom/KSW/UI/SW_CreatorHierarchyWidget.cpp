// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_CreatorHierarchyWidget.h"
#include "../CreatorMapSubsystem.h"
#include "SW_CreatorObjectSlotWidget.h"
#include "SW_CreatorHierarchyItemWidget.h"
#include "Components/ScrollBox.h"
#include "../SW_CreatorObject.h"


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

void USW_CreatorHierarchyWidget::ReloadWidget(const TArray<ASW_CreatorObject*>& InCreatorObjects, int32 depth)
{
	for (int i = 0; i < InCreatorObjects.Num(); i++)
	{
		USW_CreatorHierarchyItemWidget* ChildWidget = CreateWidget<USW_CreatorHierarchyItemWidget>(GetWorld(), SlotFactory);
		ChildWidget->SetItem(InCreatorObjects[i], depth);
		ObjectScrollBox->AddChild(ChildWidget);

		SlotWidgets.Add(ChildWidget);
		TArray<AActor*> ChildActors;
		InCreatorObjects[i]->GetAllChildActors(ChildActors);
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
