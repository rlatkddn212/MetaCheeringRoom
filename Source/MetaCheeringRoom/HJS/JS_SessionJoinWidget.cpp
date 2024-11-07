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
#include "../SHK/HG_Player.h"
#include "../KSW/CreatorStorageSubsystem.h"
#include "../KSW/CreatorMapSubsystem.h"
#include "JS_HostSlotWidget.h"
#include "Components/CanvasPanel.h"
#include "JS_EasingFunctionLib.h"
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

	SetupMapData();
}

void UJS_SessionJoinWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry,DeltaTime);
	OnMakeSessionAnimation(DeltaTime);
	OnMakePopupSessionAnimation(DeltaTime);
	OnJoinSessionAnimation(DeltaTime);
	OnJoinPopupSessionAnimation(DeltaTime);
}

void UJS_SessionJoinWidget::MenuSwitching(int32 index)
{
	WS_Menu->SetActiveWidgetIndex(index);
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

void UJS_SessionJoinWidget::SetupMapData()
{
	SB_Maps->ClearChildren();
	
	UCreatorStorageSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorStorageSubsystem>();
	TArray<FCreatorMapMetaData*> MetaDataList = system->GetCreatorMapMetaDatas();
	int32 idx = 0;
	for (FCreatorMapMetaData* MetaData : MetaDataList)
	{
		UJS_HostSlotWidget* SlotWidget = CreateWidget<UJS_HostSlotWidget>(GetWorld(), SlotFactory);
		SlotWidget->SetupInfo(MetaData, this, idx++);
		SlotWidget->SetVisibility(ESlateVisibility::Visible);
		SB_Maps->AddChild(SlotWidget);
	}
}

void UJS_SessionJoinWidget::OnClickSlot(int32 slotIdx)
{
	BD_CreateRoom->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	PlayShowMakePopupSessionAnimation();
	SelectIndex = slotIdx;
}

void UJS_SessionJoinWidget::PlayShowMakeSessionAnimation()
{
	AnimationAlpha = 0.0f;
	bMakeWidgetAnimating = true;
}

void UJS_SessionJoinWidget::OnMakeSessionAnimation(float DeltaTime)
{
	if (bMakeWidgetAnimating)
	{
		// Alpha 값 증가시키기
		AnimationAlpha += DeltaTime / AnimationDuration;
		AnimationAlpha = FMath::Clamp(AnimationAlpha, 0.0f, 1.f);  // 0~1 사이로 제한

		// 이징 함수 호출 (UI가 나타날 때 ElasticEaseOut 적용)
		float NewPosition = UJS_EasingFunctionLib::FadeInEase(AnimationAlpha);

		// NewPosition 값을 UI 위치에 적용 (예: Y 위치를 NewPosition으로 설정)
		//BD_VideoList->SetRenderTranslation(FVector2D(0, StartPosition + NewPosition * TargetOffset));
		CA_MakeSession->SetRenderOpacity(NewPosition);

		// 애니메이션이 끝나면 bIsAnimating 비활성화
		if (AnimationAlpha >= 1.0f)
		{
			bMakeWidgetAnimating = false;
		}
	}
}

void UJS_SessionJoinWidget::PlayShowMakePopupSessionAnimation()
{
	AnimationAlpha = 0.0f;
	bMakePopupAnimating = true;
	StartMakeWidgetPosition = BD_CreateRoomPopup->GetRenderTransform().Translation.Y - TargetOffset;
}

void UJS_SessionJoinWidget::OnMakePopupSessionAnimation(float DeltaTime)
{
	if (bMakePopupAnimating)
	{
		// Alpha 값 증가시키기
		AnimationAlpha += DeltaTime / AnimationDuration;
		AnimationAlpha = FMath::Clamp(AnimationAlpha, 0.0f, 1.f);  // 0~1 사이로 제한

		// 이징 함수 호출 (UI가 나타날 때 ElasticEaseOut 적용)
		float NewPosition = UJS_EasingFunctionLib::ElasticEaseOut(AnimationAlpha);
		// NewPosition 값을 UI 위치에 적용 (예: Y 위치를 NewPosition으로 설정)
		BD_CreateRoomPopup->SetRenderTranslation(FVector2D(0, StartMakeWidgetPosition + NewPosition * TargetOffset));

		// 애니메이션이 끝나면 bIsAnimating 비활성화
		if (AnimationAlpha >= 1.0f)
		{
			bMakePopupAnimating = false;
		}
	}
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
	auto* cateslot = CreateWidget<UJS_SessionSlotWidget>(this, SessionSlotFactory);

	//슬롯에서 위젯에 뿌리기
	cateslot->UpdateInfo(info);
	//부모 기억시키기
	cateslot->SetupInfo(this);
	if (info.RoomCategory == ERoomCategory::CT_ESports)
	{
		SB_ESports->AddChild(cateslot);
	}

	if (info.RoomCategory == ERoomCategory::CT_Idol)
	{
		SB_Idol->AddChild(cateslot);
	}

	if (info.RoomCategory == ERoomCategory::CT_Soccer)
	{
		SB_Soccer->AddChild(cateslot);
	}

	if (info.RoomCategory == ERoomCategory::CT_Talk)
	{
		SB_Talk->AddChild(cateslot);
	}
}

void UJS_SessionJoinWidget::ShowJoinWidget()
{
	BD_JoinRoom->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	PlayJoinPopupSessionAnimation();
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
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
}

void UJS_SessionJoinWidget::OnClickedCreateQuit()
{
	BD_CreateRoom->SetVisibility(ESlateVisibility::Hidden);
}

void UJS_SessionJoinWidget::OnClickedQuit()
{
	SetVisibility(ESlateVisibility::Hidden);
	SB_All->ClearChildren();
	SB_ESports->ClearChildren();
	SB_Soccer->ClearChildren();
	SB_Idol->ClearChildren();
	SB_Talk->ClearChildren();
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

void UJS_SessionJoinWidget::PlayJoinPopupSessionAnimation()
{
	AnimationAlpha = 0.0f;
	bJoinPopupAnimating = true;
	StartMakeWidgetPosition = BD_JoinPopup->GetRenderTransform().Translation.Y - TargetOffset;
}

void UJS_SessionJoinWidget::OnJoinPopupSessionAnimation(float DeltaTime)
{
	if (bJoinPopupAnimating)
	{
		// Alpha 값 증가시키기
		AnimationAlpha += DeltaTime / AnimationDuration;
		AnimationAlpha = FMath::Clamp(AnimationAlpha, 0.0f, 1.f);  // 0~1 사이로 제한

		// 이징 함수 호출 (UI가 나타날 때 ElasticEaseOut 적용)
		float NewPosition = UJS_EasingFunctionLib::ElasticEaseOut(AnimationAlpha);
		// NewPosition 값을 UI 위치에 적용 (예: Y 위치를 NewPosition으로 설정)
		BD_JoinPopup->SetRenderTranslation(FVector2D(0, StartMakeWidgetPosition + NewPosition * TargetOffset));

		// 애니메이션이 끝나면 bIsAnimating 비활성화
		if (AnimationAlpha >= 1.0f)
		{
			bJoinPopupAnimating = false;
		}
	}
}

void UJS_SessionJoinWidget::PlayShowJoinSessionAnimation()
{
	AnimationAlpha = 0.0f;
	bJoinWidgetAnimating = true;
}

void UJS_SessionJoinWidget::OnJoinSessionAnimation(float DeltaTime)
{
	if (bJoinWidgetAnimating)
	{
		// Alpha 값 증가시키기
		AnimationAlpha += DeltaTime / AnimationDuration;
		AnimationAlpha = FMath::Clamp(AnimationAlpha, 0.0f, 1.f);  // 0~1 사이로 제한

		// 이징 함수 호출 (UI가 나타날 때 ElasticEaseOut 적용)
		float NewPosition = UJS_EasingFunctionLib::FadeInEase(AnimationAlpha);

		// NewPosition 값을 UI 위치에 적용 (예: Y 위치를 NewPosition으로 설정)
		//BD_VideoList->SetRenderTranslation(FVector2D(0, StartPosition + NewPosition * TargetOffset));
		CA_FindSession->SetRenderOpacity(NewPosition);

		// 애니메이션이 끝나면 bIsAnimating 비활성화
		if (AnimationAlpha >= 1.0f)
		{
			bJoinWidgetAnimating = false;
		}
	}
}
