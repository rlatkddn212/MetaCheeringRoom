// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_SessionJoinWidget.h"
#include "../SHK/HG_GameInstance.h"
#include "JS_SessionGameInstanceSubSystem.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "Components/ComboBoxString.h"
#include "MetaCheeringRoom.h"
#include "JS_SessionSlotWidget.h"
#include "Components/WidgetSwitcher.h"
void UJS_SessionJoinWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UHG_GameInstance* gi = Cast<UHG_GameInstance>(GetWorld()->GetGameInstance());
	si = gi->GetSubsystem<UJS_SessionGameInstanceSubSystem>();
	if (si)
	{
		si->OnSearchSignatureCompleteDelegate.AddDynamic(this,&UJS_SessionJoinWidget::AddSessionSlotWidget);
	}

	// 세션 생성 UI 초기 처리
	//BD_CreateRoom->SetVisibility(ESlateVisibility::Hidden);
	S_PlayerCount->OnValueChanged.AddDynamic(this,&UJS_SessionJoinWidget::OnSliderValueChanged);
	BTN_Create->OnClicked.AddDynamic(this,&UJS_SessionJoinWidget::CreateSession);
	BD_JoinRoom->SetVisibility(ESlateVisibility::Hidden);
	//BD_CreateRoom->SetVisibility(ESlateVisibility::Hidden);

	BTN_All->OnClicked.AddDynamic(this, &UJS_SessionJoinWidget::OnClickedCateAll);
	BTN_ESports->OnClicked.AddDynamic(this, &UJS_SessionJoinWidget::OnClickedCateESports);
	BTN_Soccer->OnClicked.AddDynamic(this, &UJS_SessionJoinWidget::OnClickedCateSoccer);
	BTN_Idol->OnClicked.AddDynamic(this, &UJS_SessionJoinWidget::OnClickedCateIdol);
	BTN_Talk->OnClicked.AddDynamic(this, &UJS_SessionJoinWidget::OnClickedCateTalk);
	BTN_Refresh->OnClicked.AddDynamic(this, &UJS_SessionJoinWidget::OnClickedRefresh);
	BTN_Join->OnClicked.AddDynamic(this, &UJS_SessionJoinWidget::OnClickedJoinRoom);
	BTN_JoinQuit->OnClicked.AddDynamic(this, &UJS_SessionJoinWidget::OnClickedJoinQuit);
	BTN_JoinBack->OnClicked.AddDynamic(this, &UJS_SessionJoinWidget::OnClickedJoinQuit);
	BTN_Quit->OnClicked.AddDynamic(this, &UJS_SessionJoinWidget::OnClickedQuit);
	BTN_CreateQuit->OnClicked.AddDynamic(this, &UJS_SessionJoinWidget::OnClickedCreateQuit);
}

void UJS_SessionJoinWidget::MenuSwitching(int32 index)
{
	WS_Menu->SetActiveWidgetIndex(index);
}

void UJS_SessionJoinWidget::OnSliderValueChanged(float value)
{
	int intValue = FMath::FloorToInt(value);
	TEXT_PlayerCount->SetText(FText::AsNumber(intValue));
}

void UJS_SessionJoinWidget::CreateSession()
{

	//
	FString roomName = ED_RoomName->GetText().ToString();
	roomName = roomName.TrimStartAndEnd();
	if (roomName.IsEmpty())
	{
		return;
	}
	int32 PlayerCount = (int32)S_PlayerCount->GetValue();
	PRINTLOG(TEXT("%s"), *CB_Category->GetSelectedOption());
	//일단 4로 진행
	si->CreateSession(roomName, PlayerCount, CB_Category->GetSelectedOption());
}

void UJS_SessionJoinWidget::AddSessionSlotWidget(const FRoomInfo& info)
{
	// 슬롯을 생성해서 스크롤박스에 붙이고 싶다.(추가하고 싶다)
	// 카테고리별로 나눠서 추가하기
		//슬롯을 생성해서 스크롤박스에 붙이고 싶다.(추가하고 싶다)
	auto* slot = CreateWidget<UJS_SessionSlotWidget>(this, SessionSlotFactory);

	//슬롯에서 위젯에 뿌리기
	slot->UpdateInfo(info);
	//부모 기억시키기
	slot->SetupInfo(this);
	//스크롤박스에 붙이기
	SB_All->AddChild(slot);
	
	if (info.RoomCategory == ERoomCategory::CT_ESports)
	{
		SB_ESports->AddChild(slot);
	}
	if (info.RoomCategory == ERoomCategory::CT_Idol)
	{
		SB_Idol->AddChild(slot);
	}
	if (info.RoomCategory == ERoomCategory::CT_Soccer)
	{
		SB_Soccer->AddChild(slot);
	}
	if (info.RoomCategory == ERoomCategory::CT_Talk)
	{
		SB_Talk->AddChild(slot);
	}
}

void UJS_SessionJoinWidget::ShowJoinWidget()
{
	BD_JoinRoom->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UJS_SessionJoinWidget::CategorySwitching(int32 index)
{
	WS_Category->SetActiveWidgetIndex(index);
}

void UJS_SessionJoinWidget::OnClickedCateAll()
{
	CategorySwitching(0);
}

void UJS_SessionJoinWidget::OnClickedCateESports()
{
	CategorySwitching(1);
}

void UJS_SessionJoinWidget::OnClickedCateSoccer()
{
	CategorySwitching(2);
}

void UJS_SessionJoinWidget::OnClickedCateIdol()
{
	CategorySwitching(3);
}

void UJS_SessionJoinWidget::OnClickedCateTalk()
{
	CategorySwitching(4);
}

void UJS_SessionJoinWidget::OnClickedRefresh()
{
	//기존 스크롤박스 목록 삭제하고
	SB_All->ClearChildren();
	SB_ESports->ClearChildren();
	SB_Soccer->ClearChildren();
	SB_Idol->ClearChildren();
	SB_Talk->ClearChildren();
	//방찾기를 수행
	if (si)
	{
		si->FindOtherSessions();
	}
}

void UJS_SessionJoinWidget::OnClickedJoinRoom()
{
	if (LobbySelectedIndex != -1)
	{
		si->JoinToSession(LobbySelectedIndex);
	}
}

void UJS_SessionJoinWidget::OnClickedJoinQuit()
{
	BD_JoinRoom->SetVisibility(ESlateVisibility::Hidden);
	LobbySelectedIndex = -1;
}

void UJS_SessionJoinWidget::OnClickedCreateQuit()
{
	BD_CreateRoom->SetVisibility(ESlateVisibility::Hidden);
}

void UJS_SessionJoinWidget::OnClickedQuit()
{
	SetVisibility(ESlateVisibility::Hidden);
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	PC->SetShowMouseCursor(false);
	PC->SetInputMode(FInputModeGameOnly());
}
