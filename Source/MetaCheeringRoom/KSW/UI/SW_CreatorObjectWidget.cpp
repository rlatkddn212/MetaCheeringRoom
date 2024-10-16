// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_CreatorObjectWidget.h"
#include "SW_CreatorObjectSlotWidget.h"
#include "../CreatorStorageSubsystem.h"
#include "Components/ScrollBox.h"

void USW_CreatorObjectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UCreatorStorageSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorStorageSubsystem>();
	TArray<FCreatorObjectData*> CreatorObjects = system->GetCreatorObjects();

	for (int i = 0; i < CreatorObjects.Num(); i++)
	{
		USW_CreatorObjectSlotWidget* ChildWidget = CreateWidget<USW_CreatorObjectSlotWidget>(GetWorld(), SlotFactory);
		ChildWidget->SetObject(CreatorObjects[i]);
		ObjectScrollBox->AddChild(ChildWidget);
	}
}
