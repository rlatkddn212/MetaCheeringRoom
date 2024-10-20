// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_SessionSlotWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "JS_SessionJoinWidget.h"

void UJS_SessionSlotWidget::SetupInfo(class UJS_SessionJoinWidget* parent)
{
	Parent = parent;
	BTN_Join->OnClicked.AddDynamic(this,&UJS_SessionSlotWidget::OnJoinButtonClicked);
}



void UJS_SessionSlotWidget::UpdateInfo(const FRoomInfo& info)
{
	TEXT_RoomName->SetText(FText::FromString(info.RoomName));
	TEXT_HostName->SetText(FText::FromString(info.HostName));
	FString Count = FString::Printf(TEXT("%d / %d"), info.CurrentPlayerCount, info.MaxPlayerCount);
	TEXT_PlayerCount->SetText(FText::FromString(Count));

	SessionSearchIndex = info.Index;
}

void UJS_SessionSlotWidget::OnJoinButtonClicked()
{
	if (Parent)
	{
		Parent->LobbySelectedIndex = SessionSearchIndex;
		Parent->ShowJoinWidget();
	}
}
