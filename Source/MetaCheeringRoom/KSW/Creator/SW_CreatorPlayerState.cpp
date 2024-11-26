// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/Creator/SW_CreatorPlayerState.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/OnlineSubsystem.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineIdentityInterface.h"
#include "Online/CoreOnlineFwd.h"
#include "Kismet/GameplayStatics.h"

void ASW_CreatorPlayerState::BeginPlay()
{
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
                // 유니크 넷 ID를 문자열로 변환
                FString Nickname = IdentityInterface->GetPlayerNickname(*UserId);
                ServerRPCSetPlayerName(Nickname);
            }
        }
    }
}

void ASW_CreatorPlayerState::ServerRPCSetPlayerName_Implementation(const FString& newName)
{
    SetPlayerName(newName);
}
