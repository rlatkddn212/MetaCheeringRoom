// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_CreateCreatorSlotWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../CreatorStorageSubsystem.h"
#include "SW_CreateCreatorWidget.h"
#include "Components/Image.h"
#include "Util/UtilBlueprintFunctionLibrary.h"

void USW_CreateCreatorSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_Join->OnClicked.AddDynamic(this, &USW_CreateCreatorSlotWidget::OnJoin);
}

void USW_CreateCreatorSlotWidget::SetupInfo(FCreatorMapMetaData* metaData, USW_CreateCreatorWidget* parent, int32 idx)
{
	slotIdx = idx;
	Parent = parent;
	MapMetaData = metaData;
	TEXT_Creator->SetText(FText::FromString(metaData->CreatorName));
	TEXT_Date->SetText(FText::FromString(metaData->CreatedTime.ToString()));
	TEXT_MapName->SetText(FText::FromString(metaData->CreatorMapName));
	UTexture2D* Texture = UUtilBlueprintFunctionLibrary::LoadScreenshotAsTexture(metaData->ThumbnailFileName);

	IMG_Thumbnail->SetBrushFromTexture(Texture);
}

void USW_CreateCreatorSlotWidget::OnJoin()
{
	Parent->OnClickSlot(slotIdx);
}
