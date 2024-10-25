// Fill out your copyright notice in the Description page of Project Settings.

#include "KSW/UI/SW_CreateCreatorWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/EditableText.h"
#include "Kismet/GameplayStatics.h"
#include "../CreatorMapSubsystem.h"
#include "../CreatorStorageSubsystem.h"
#include "SW_CreateCreatorSlotWidget.h"
#include "Components/ScrollBox.h"

void USW_CreateCreatorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BTN_Quit->OnClicked.AddDynamic(this, &USW_CreateCreatorWidget::OnBack);
	BTN_Create->OnClicked.AddDynamic(this, &USW_CreateCreatorWidget::OnCreate);
	BTN_Back->OnClicked.AddDynamic(this, &USW_CreateCreatorWidget::OnBack);

	BTN_CreateLevel->OnClicked.AddDynamic(this, &USW_CreateCreatorWidget::OnCreateLevel);
	BTN_BackLevel->OnClicked.AddDynamic(this, &USW_CreateCreatorWidget::OnBackLevel);

	BTN_BackChange->OnClicked.AddDynamic(this, &USW_CreateCreatorWidget::OnBackChange);
	BTN_Change->OnClicked.AddDynamic(this, &USW_CreateCreatorWidget::OnChange);
	BTN_Remove->OnClicked.AddDynamic(this, &USW_CreateCreatorWidget::OnRemove);

	SetupInfo();
}

void USW_CreateCreatorWidget::SetupInfo()
{
	// ��� ����
	for (USW_CreateCreatorSlotWidget* SlotWidget : SlotWidgets)
	{
		SlotWidget->RemoveFromParent();
	}

	// �� ������ �����ͼ� ������ �����ϴ� �Լ� ȣ��
	UCreatorStorageSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorStorageSubsystem>();
	TArray<FCreatorMapMetaData*> MetaDataList = system->GetCreatorMapMetaDatas();
	int32 idx = 0;
	for (FCreatorMapMetaData* MetaData : MetaDataList)
	{
		// ������ �����ϴ� �Լ� ȣ��
		USW_CreateCreatorSlotWidget* SlotWidget = CreateWidget<USW_CreateCreatorSlotWidget>(GetWorld(), SlotFactory);
		SlotWidget->SetupInfo(MetaData, this, idx++);
		SlotWidgets.Add(SlotWidget);
		SB_Maps->AddChild(SlotWidget);
	}
}

void USW_CreateCreatorWidget::OnCreate()
{
	BD_CreateLevel->SetVisibility(ESlateVisibility::Visible);

}

void USW_CreateCreatorWidget::OnBack()
{
	// �� ������ �ݴ´�.
	this->RemoveFromParent();
}

void USW_CreateCreatorWidget::OnCreateLevel()
{
	// ������ �ؽ�Ʈ���� �ؽ�Ʈ�� �����ͼ� ������ �����ϴ� �Լ��� ȣ��

	FString NewLevelName = ED_LevelName->GetText().ToString();

	UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
	system->SetupJson("[]");
	system->SetMapName(NewLevelName);
	// ���� ���� �Լ� ȣ��
	UGameplayStatics::OpenLevel(GetWorld(), LevelName, true);
}

void USW_CreateCreatorWidget::OnBackLevel()
{
	BD_CreateLevel->SetVisibility(ESlateVisibility::Hidden);
}

void USW_CreateCreatorWidget::OnBackChange()
{
	BD_ChangeLevel->SetVisibility(ESlateVisibility::Hidden);
}

void USW_CreateCreatorWidget::OnChange()
{
	UCreatorStorageSubsystem* storage = GetGameInstance()->GetSubsystem<UCreatorStorageSubsystem>();
	TArray<FCreatorMapMetaData*> meta = storage->GetCreatorMapMetaDatas();

	FString path = FPaths::ProjectContentDir() + meta[SelectSlotIdx]->FileName;
	FString JsonStr = storage->LoadCreatorMap(path);
	
	UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
	system->SetMapName(meta[SelectSlotIdx]->CreatorMapName);
	system->SetupJson(JsonStr);

	UGameplayStatics::OpenLevel(GetWorld(), LevelName, true);
}

void USW_CreateCreatorWidget::OnRemove()
{
	UCreatorStorageSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorStorageSubsystem>();
	system->RemoveMetaData(SelectSlotIdx);

	BD_ChangeLevel->SetVisibility(ESlateVisibility::Hidden);
	SetupInfo();
}

void USW_CreateCreatorWidget::OnClickSlot(int32 slotIdx)
{
	// ���õ� ������ �� �̸��� �����ͼ� ������ �����ϴ� �Լ��� ȣ��
	BD_ChangeLevel->SetVisibility(ESlateVisibility::Visible);
	SelectSlotIdx = slotIdx;
}
