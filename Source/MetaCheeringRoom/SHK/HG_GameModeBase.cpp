// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_GameModeBase.h"
#include "../HJS/JS_SessionJoinWidget.h"
#include "HG_GameInstance.h"
#include "../HJS/JS_SessionGameInstanceSubSystem.h"
#include "../KSW/UI/SW_CreateCreatorWidget.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystemUtils.h"
#include "MetaCheeringRoom.h"
#include "../HJS/JS_CreateRoomWidget.h"
void AHG_GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (CreateRoomWidgetFactory)
	{
		CreateRoomWidget = CreateWidget<UJS_CreateRoomWidget>(GetWorld(), CreateRoomWidgetFactory);
		CreateRoomWidget->AddToViewport();
		CreateRoomWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (SessionWidgetFactory)
	{
		SessionWidget = CreateWidget<UJS_SessionJoinWidget>(GetWorld(), SessionWidgetFactory);
		SessionWidget->AddToViewport();
		SessionWidget->SetVisibility(ESlateVisibility::Hidden);
	}


	if (CreateCreatorWidgetFactory)
	{
		CreateCreatorWidget = CreateWidget<USW_CreateCreatorWidget>(GetWorld(), CreateCreatorWidgetFactory);
		CreateCreatorWidget->AddToViewport();
		CreateCreatorWidget->SetVisibility(ESlateVisibility::Hidden);
	}


	UHG_GameInstance* gi = Cast<UHG_GameInstance>(GetWorld()->GetGameInstance());
	if (gi)
	{
		UJS_SessionGameInstanceSubSystem* si = gi->GetSubsystem<UJS_SessionGameInstanceSubSystem>();
		if (si)
		{
			// Online Subsystem 인스턴스 가져오기
			IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get(TEXT("Steam"));
			if (SubSystem)
			{
				// Identity 인터페이스 가져오기
				IOnlineIdentityPtr IdentityInterface = SubSystem->GetIdentityInterface();
				if (IdentityInterface.IsValid())
				{
					// 로컬 플레이어의 유니크 넷 ID 가져오기
					FUniqueNetIdPtr UserId = IdentityInterface->GetUniquePlayerId(0);
					if (UserId.IsValid())
					{
						// Steam ID를 문자열로 변환
						si->PlayerName = FName(UserId->ToString());
					}
				}
			}
			si->FindOtherSessions();
		}
	}
}
