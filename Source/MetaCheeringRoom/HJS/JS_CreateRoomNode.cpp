// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_CreateRoomNode.h"
#include "Components/Button.h"
#include "JS_CreateRoomWidget.h"
#include "Components/TextBlock.h"
#include "../KSW/CreatorStorageSubsystem.h"

void UJS_CreateRoomNode::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_Select->OnClicked.AddDynamic(this, &UJS_CreateRoomNode::OnSelect);
	BTN_Hosting->OnClicked.AddDynamic(this, &UJS_CreateRoomNode::OnHosting);
	BTN_Remove->OnClicked.AddDynamic(this, &UJS_CreateRoomNode::OnRemove);
	BTN_Modify->OnClicked.AddDynamic(this, &UJS_CreateRoomNode::OnModify);
	
}

void UJS_CreateRoomNode::OnSelect()
{
	if (Parent)
	{
		Parent->OnClickSlot(slotIdx);
		PlayAnimation(SelectAnim);
	}
}

void UJS_CreateRoomNode::OnHosting()
{
	if (Parent)
	{
		Parent->OnClickHosting();
	}
}

void UJS_CreateRoomNode::OnRemove()
{
	if (Parent)
	{
		Parent->OnClickRemove();
	}
}

void UJS_CreateRoomNode::OnModify()
{
	if (Parent)
	{
		Parent->OnClickModify();
	}
}

void UJS_CreateRoomNode::SetupInfo(struct FCreatorMapMetaData* metaData, class UJS_CreateRoomWidget* parent, int32 idx)
{
	slotIdx = idx;
	Parent = parent;
	MapMetaData = metaData;
	TEXT_MapName->SetText(FText::FromString(metaData->CreatorMapName));
}
