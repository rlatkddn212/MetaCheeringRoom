// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_HostSlotWidget.h"
#include "../KSW/CreatorStorageSubsystem.h"
#include "JS_SessionJoinWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "../MetaCheeringRoom.h"

void UJS_HostSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_HostMap->OnClicked.AddDynamic(this, &UJS_HostSlotWidget::OnClickHostBtn);
}

void UJS_HostSlotWidget::SetupInfo(struct FCreatorMapMetaData* metaData, class UJS_SessionJoinWidget* parent, int32 idx)
{
	slotIdx = idx;
	SParent = parent;
	MapMetaData = metaData;
	TEXT_Creator->SetText(FText::FromString(metaData->CreatorName));
	TEXT_Date->SetText(FText::FromString(metaData->CreatedTime.ToString()));
	TEXT_MapName->SetText(FText::FromString(metaData->CreatorMapName));
}

void UJS_HostSlotWidget::OnClickHostBtn()
{
	PRINTLOG(TEXT("1111"));
	SParent->OnClickSlot(slotIdx);
}
