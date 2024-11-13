// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_HostSlotWidget.h"
#include "../KSW/CreatorStorageSubsystem.h"
#include "JS_SessionJoinWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "../MetaCheeringRoom.h"
#include "JS_CreateRoomWidget.h"
#include "../Util/UtilBlueprintFunctionLibrary.h"
#include "Components/Image.h"

void UJS_HostSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_HostMap->OnClicked.AddDynamic(this, &UJS_HostSlotWidget::OnClickHostBtn);
}

void UJS_HostSlotWidget::SetupInfo(FCreatorMapMetaData* metaData, UJS_CreateRoomWidget* parent, int32 idx)
{
	slotIdx = idx;
	SParent = parent;
	MapMetaData = metaData;
	TEXT_Creator->SetText(FText::FromString(metaData->CreatorName));
	TEXT_Date->SetText(FText::FromString(metaData->CreatedTime.ToString()));
	TEXT_MapName->SetText(FText::FromString(metaData->CreatorMapName));
	UTexture2D* Texture = UUtilBlueprintFunctionLibrary::LoadScreenshotAsTexture(metaData->ThumbnailFileName);
	if (Texture)
		IMG_Thumbnail->SetBrushFromTexture(Texture);
}

void UJS_HostSlotWidget::OnClickHostBtn()
{
	SParent->OnClickSlot(slotIdx);
}