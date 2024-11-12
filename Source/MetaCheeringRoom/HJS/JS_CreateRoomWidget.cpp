// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_CreateRoomWidget.h"
#include "../SHK/HG_GameInstance.h"
#include "JS_SessionGameInstanceSubSystem.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/ComboBoxString.h"
#include "MetaCheeringRoom.h"
#include "JS_SessionSlotWidget.h"
#include "Components/WidgetSwitcher.h"
#include "../SHK/HG_Player.h"
#include "../KSW/CreatorStorageSubsystem.h"
#include "../KSW/CreatorMapSubsystem.h"
#include "JS_HostSlotWidget.h"
#include "Components/CanvasPanel.h"
#include "JS_EasingFunctionLib.h"
#include "JS_CreateRoomNode.h"



void UJS_CreateRoomWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UHG_GameInstance* gi = Cast<UHG_GameInstance>(GetWorld()->GetGameInstance());
	si = gi->GetSubsystem<UJS_SessionGameInstanceSubSystem>();

	// 세션 생성 UI 초기 처리
	BTN_Hosting->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::CreateSession);
	BTN_Quit->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::OnClickedQuit);
	BTN_HostingCancel->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::OnClickedCreateQuit);
	
	SetupMapData();
}

void UJS_CreateRoomWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry,DeltaTime);
}

void UJS_CreateRoomWidget::PlayShowWidgetAnimation()
{
	PlayAnimation(ShowWidget);
}

void UJS_CreateRoomWidget::OnClickedQuit()
{
	// 나중에는 그냥 끄지 말고 타이머로 끄든가 합시다.
	SetVisibility(ESlateVisibility::Hidden);
	// 창 닫기
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	PC->SetShowMouseCursor(false);
	PC->SetInputMode(FInputModeGameOnly());
	AHG_Player* Player = Cast<AHG_Player>(PC->GetCharacter());
	if (Player)
	{
		Player->Direction = FVector::ZeroVector;
		Player->bCanMove = true;
	}
}

void UJS_CreateRoomWidget::CreateSession()
{
	FString roomName = ED_RoomName->GetText().ToString();
	roomName = roomName.TrimStartAndEnd();
	if (roomName.IsEmpty())
	{
		return;
	}

	int32 PlayerCount = 30;
	PRINTLOG(TEXT("%s"), *CB_Category->GetSelectedOption());

	FString category;
	switch (CB_Category->GetSelectedIndex())
	{
	case 0:
		category = TEXT("ESports");
		break;
	case 1:
		category = TEXT("Soccer");
		break;
	case 2:
		category = TEXT("Idol");
		break;
	case 3:
		category = TEXT("Talk");
		break;
	}
	UCreatorStorageSubsystem* storage = GetGameInstance()->GetSubsystem<UCreatorStorageSubsystem>();
	TArray<FCreatorMapMetaData*> meta = storage->GetCreatorMapMetaDatas();

	FString path = FPaths::ProjectSavedDir() + TEXT("/CreatorMap/") + meta[SelectIndex]->FileName;
	FString JsonStr = storage->LoadCreatorMap(path);

	UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
	system->SetMapName(meta[SelectIndex]->CreatorMapName);
	system->SetupJson(JsonStr);

	si->CreateSession(roomName, PlayerCount, CB_Category->GetSelectedOption());
}

void UJS_CreateRoomWidget::SetupMapData()
{
	SB_Maps->ClearChildren();

	UCreatorStorageSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorStorageSubsystem>();
	TArray<FCreatorMapMetaData*> MetaDataList = system->GetCreatorMapMetaDatas();
	int32 idx = 0;
	for (FCreatorMapMetaData* MetaData : MetaDataList)
	{
		UJS_CreateRoomNode* SlotWidget = CreateWidget<UJS_CreateRoomNode>(GetWorld(), SlotFactory);
		if (SlotWidget)
		{
			SlotWidget->SetupInfo(MetaData, this, idx++);
			SlotWidget->SetVisibility(ESlateVisibility::Visible);
			SlotWidget->Padding.Bottom = 10.f;
			SB_Maps->AddChild(SlotWidget);
		}
	}
}

void UJS_CreateRoomWidget::OnClickSlot(int32 slotIdx)
{
	// 기존 선택 슬롯 애니메이션 역재생
	UJS_CreateRoomNode* CurrentSlot = Cast<UJS_CreateRoomNode>(SB_Maps->GetChildAt(SelectIndex));
	if (CurrentSlot)
	{
		// 애니메이션 역재생
		CurrentSlot->PlayAnimation(CurrentSlot->SelectAnim,0.f,1,EUMGSequencePlayMode::Reverse);
	}

	// 슬롯 애니메이션 재생은 슬롯에서

	// 클릭한 슬롯 선택 처리
	SelectIndex = slotIdx;
}

void UJS_CreateRoomWidget::OnClickHosting()
{
	// 호스팅 팝업 열기 ( 애니메이션 )
}

void UJS_CreateRoomWidget::OnClickModify()
{
	// 수정 팝업 열기 ( 애니메이션 )
}

void UJS_CreateRoomWidget::OnClickRemove()
{
	// 삭제 팝업 열기 ( 애니메이션 )
}

void UJS_CreateRoomWidget::OnClickedCreateQuit()
{
	// 애니메이션 역재생
	PlayAnimation(HostingLevelPopup,0.f,1,EUMGSequencePlayMode::Reverse);
}
