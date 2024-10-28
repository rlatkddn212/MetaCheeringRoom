// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_CreatorObjectWidget.h"
#include "SW_CreatorObjectSlotWidget.h"
#include "../CreatorStorageSubsystem.h"
#include "Components/ScrollBox.h"

void USW_CreatorObjectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UCreatorStorageSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorStorageSubsystem>();
	TMap<int32, FCreatorObjectData*> CreatorObjects = system->GetCreatorObjects(1);
	for (auto& CreatorObject : CreatorObjects)
	{
		USW_CreatorObjectSlotWidget* ChildWidget = CreateWidget<USW_CreatorObjectSlotWidget>(GetWorld(), SlotFactory);
		ChildWidget->SetObject(CreatorObject.Value);
		ObjectScrollBox->AddChild(ChildWidget);
	}
}
