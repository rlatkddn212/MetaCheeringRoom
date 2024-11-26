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
        // Identity �������̽� ��������
        IOnlineIdentityPtr IdentityInterface = SubSystem->GetIdentityInterface();
        if (IdentityInterface.IsValid())
        {
            // ���� �÷��̾��� ����ũ �� ID ��������
            FUniqueNetIdPtr UserId = IdentityInterface->GetUniquePlayerId(0);
            if (UserId.IsValid())
            {
                // ����ũ �� ID�� ���ڿ��� ��ȯ
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
