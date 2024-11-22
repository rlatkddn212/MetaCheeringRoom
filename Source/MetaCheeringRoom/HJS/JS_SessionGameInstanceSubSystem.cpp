// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_SessionGameInstanceSubSystem.h"
#include "OnlineSubSystem.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "../../../../Plugins/Online/OnlineBase/Source/Public/Online/OnlineSessionNames.h"
#include "Online/CoreOnline.h"
#include "Containers/StringConv.h"
#include <string>
#include "MetaCheeringRoom.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystemUtils.h"
#include "JS_PlayerController.h"
//세션 이름

void UJS_SessionGameInstanceSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();
	}
	if (SessionInterface)
	{
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UJS_SessionGameInstanceSubSystem::OnMyCreateSessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UJS_SessionGameInstanceSubSystem::OnMyFindSessionComplete);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UJS_SessionGameInstanceSubSystem::OnMyJoinSessionComplete);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UJS_SessionGameInstanceSubSystem::OnMyDestroySessionComplete);
	}

}

void UJS_SessionGameInstanceSubSystem::CreateSession(const FString& RoomName, int32 PlayerCount, const FString& Category)
{
	if (SessionInterface)
	{
		FOnlineSessionSettings SessionSettings;

		//로비 사용
		SessionSettings.bUseLobbiesIfAvailable = true;

		//1.데디케이트 서버인가?
		SessionSettings.bIsDedicated = false;
		PRINTLOG(TEXT("%s"), *IOnlineSubsystem::Get()->GetSubsystemName().ToString());
		//2.Lan으로 매치하는가? , 서브시스템이 널이라면 랜매치겠죠? 아니면 스팀이라 뜰꺼니까
		SessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";

		//3. 매칭이 공개인지 & 비공개인지
		SessionSettings.bShouldAdvertise = true;//공개

		//4. 유저의 상태 정보(온라인/ 자리비움 등등) 사용여부
		SessionSettings.bUsesPresence = true;

		//5. 중간에 난입가능한가?
		SessionSettings.bAllowJoinInProgress = true;
		SessionSettings.bAllowJoinViaPresence = true;

		//6. 몇명이 접속할래?
		SessionSettings.NumPublicConnections = PlayerCount;

		// 세션 설정 시 카테고리 지정 -> 추후에 
		FString roomname = StringBase64Encode(RoomName);
		SessionSettings.Set<FString>(FName("Category"), Category, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionSettings.Set<FString>(FName("Room_Name"), roomname, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionSettings.Set<FString>(FName("Host_Name"), PlayerName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		// SessionSettings에서 값을 가져와 로그로 출력

		if (SessionSettings.Get<FString>(FName("Category"), CategoryValue))
		{
			UE_LOG(LogTemp, Warning, TEXT("Category: %s"), *CategoryValue);
		}

		if (SessionSettings.Get<FString>(FName("Room_Name"), RoomNameValue))
		{
			UE_LOG(LogTemp, Warning, TEXT("Room Name: %s"), *RoomNameValue);
		}

		if (SessionSettings.Get<FString>(FName("Host_Name"), HostNameValue))
		{
			UE_LOG(LogTemp, Warning, TEXT("Host Name: %s"), *HostNameValue);
		}

		FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();
		SessionInterface->CreateSession(*netID, FName(HostNameValue), SessionSettings);
	}
}

void UJS_SessionGameInstanceSubSystem::FindOtherSessions()
{

	UE_LOG(LogTemp, Warning, TEXT("HName"));

	SessionSearch = MakeShareable(new FOnlineSessionSearch);

	//무슨조건으로 찾을래
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	//랜인가?
	SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	//몇 개 찾을래
	SessionSearch->MaxSearchResults = 40;
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UJS_SessionGameInstanceSubSystem::JoinToSession(int32 Index)
{
	UE_LOG(LogTemp, Warning, TEXT("Joining"));
	auto result = SessionSearch->SearchResults[Index];
	SessionInterface->JoinSession(0, FName(PlayerName), result);
}

void UJS_SessionGameInstanceSubSystem::DestroySession()
{
}

void UJS_SessionGameInstanceSubSystem::OnMyCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		PRINTLOG(TEXT("Failed"));
		return;
	}
	// 서버 트래블하기 ( 영상 맵으로 )
	FOnlineSessionSettings* SessionSettings = SessionInterface->GetSessionSettings(SessionName);
	
	if (CategoryValue == "Creator")
	{
		PRINTLOG(TEXT("Creator"));
		GetWorld()->ServerTravel("/Game/Ksw/Maps/KswTestMap?listen");
	}
	else
	{
		PRINTLOG(TEXT("No Creator"));
		GetWorld()->ServerTravel("/Game/HJS/Maps/HJSTestMap?listen");
	}

	PRINTLOG(TEXT("Success"));
	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

}

void UJS_SessionGameInstanceSubSystem::OnMyDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		//로비로 여행을 가고 싶다.
		auto* pc = GetWorld()->GetFirstPlayerController();
		pc->ClientTravel(TEXT("/Game/SHK/HG_LobbyLevel"), ETravelType::TRAVEL_Absolute);
	}
}

void UJS_SessionGameInstanceSubSystem::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	LoadServerWidgetMap();
}

void UJS_SessionGameInstanceSubSystem::LoadServerWidgetMap()
{
	// AKJH_PlayerController를 가져온다,
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	//// 플레이어의 첫번째 컨트롤러를 가져온다.
	//APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (PC && PC->IsLocalController()) // 컨트롤러가 있으면,
	{
		// ServerUI가 있는 맵으로 이동시킨다.
		PC->ClientTravel("/Game/JJH/MAP_Reallobby_SHN", ETravelType::TRAVEL_Absolute);
		UE_LOG(LogTemp, Error, TEXT("Session Destroy Network Failure"));
	}
}

FString UJS_SessionGameInstanceSubSystem::StringBase64Encode(const FString& str)
{
	// Set 할 때 : FString -> UTF8 (std::string) -> TArray<uint8> -> base64 로 Encode
	std::string utf8String = TCHAR_TO_UTF8(*str);
	TArray<uint8> arrayData = TArray<uint8>((uint8*)(utf8String.c_str()),
		utf8String.length());
	return FBase64::Encode(arrayData);
}

FString UJS_SessionGameInstanceSubSystem::StringBase64Decode(const FString& str)
{
	// Get 할 때 : base64 로 Decode -> TArray<uint8> -> TCHAR
	TArray<uint8> arrayData;
	FBase64::Decode(str, arrayData);
	std::string ut8String((char*)(arrayData.GetData()), arrayData.Num());
	return UTF8_TO_TCHAR(ut8String.c_str());
}

void UJS_SessionGameInstanceSubSystem::OnMyFindSessionComplete(bool Success)
{
	if (SessionSearch)
	{
		if (Success && SessionSearch->SearchResults.Num() > 0)
		{
			//방 만든 결과들 배열에 담아서
			TArray<FOnlineSessionSearchResult> results = SessionSearch->SearchResults;
			for (int32 i = 0; i < results.Num(); i++)
			{
				if (false == results[i].IsValid())
				{
					continue;
				}
				//방 정보
				FRoomInfo roomInfo;
				//카테고리
				//카테고리 NN인 것만 나눠서 Find창에 띄우기
				FString categoryString;
				if (results[i].Session.SessionSettings.Get<FString>(FName("Category"), categoryString))
				{
					if (categoryString == "ESports")
						roomInfo.RoomCategory = ERoomCategory::CT_ESports;
					else if (categoryString == "Soccer")
						roomInfo.RoomCategory = ERoomCategory::CT_Soccer;
					else if (categoryString == "Idol")
						roomInfo.RoomCategory = ERoomCategory::CT_Idol;
					else if (categoryString == "Creator")
						roomInfo.RoomCategory = ERoomCategory::CT_Creator;
				}

				roomInfo.Index = i;
				//방이름
				FString roomNameString;
				results[i].Session.SessionSettings.Get<FString>(FName("Room_Name"), roomNameString);
				roomInfo.RoomName = StringBase64Decode(roomNameString);
				//호스트 이름
				FString hostNameString;
				results[i].Session.SessionSettings.Get<FString>(FName("Host_Name"), hostNameString);
				roomInfo.HostName = hostNameString;
				//최대 플레이어 수
				roomInfo.MaxPlayerCount = results[i].Session.SessionSettings.NumPublicConnections;
				//입장 가능한 플레이어 최대 - 지금
				roomInfo.CurrentPlayerCount = roomInfo.MaxPlayerCount - results[i].Session.NumOpenPublicConnections;
				//핑 정보
				roomInfo.PingMS = results[i].PingInMs;


				//델리게이트 룸 생성
				if (OnSearchSignatureCompleteDelegate.IsBound()) OnSearchSignatureCompleteDelegate.Broadcast(roomInfo);
			}
		}
		else
		{
			// 검색 결과가 없을 때 처리
			FRoomInfo EmptyRoomInfo;
			EmptyRoomInfo.RoomName = TEXT("입장 가능한 방이 없습니다.");
			OnSearchSignatureCompleteDelegate.Broadcast(EmptyRoomInfo);
		}
	}
}

void UJS_SessionGameInstanceSubSystem::OnMyJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		//서버가 있는 레벨로 여행을 떠나고 싶다
		APlayerController* pc = GetWorld()->GetFirstPlayerController();

		FString url;
		SessionInterface->GetResolvedConnectString(SessionName, url);
		if (false == url.IsEmpty())
		{
			pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);
		}

	}
}

void UJS_SessionGameInstanceSubSystem::ExitSession()
{
	if (GetWorld()->IsNetMode(NM_Client))
	{
		ClientLeaveSession();
	}
	else
	{
		ServerRPCExitSession();
	}
}

void UJS_SessionGameInstanceSubSystem::ServerRPCExitSession_Implementation()
{
	MulticastRPCExitSession(PlayerName);
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AJS_PlayerController* PC = Cast<AJS_PlayerController>(It->Get());
		PRINTLOG(TEXT("0123456789"));
		if (PC)
		{
			PRINTLOG(TEXT("9876543210"));
			PC->ClientSessionExit();
		}
	}
	GetWorld()->GetTimerManager().SetTimer(ExitTimerHandle,this,&UJS_SessionGameInstanceSubSystem::ServerExit,1.f,false);
}

void UJS_SessionGameInstanceSubSystem::MulticastRPCExitSession_Implementation(const FName& playerName)
{
	PRINTLOG(TEXT("GameInstance Multicast Called!!!"));
	if (nullptr != GEngine)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UJS_SessionGameInstanceSubSystem::OnNetworkFailure);
	}
	//방퇴장 요청
	//클라이언트 입장에서는 그냥 나가는 거 호스트 입장에선 파괴하는거
	//SessionInterface->DestroySession(SessionName);

	// 모든 클라이언트에게 세션이 종료되었음을 알림
	if (GetWorld()->IsNetMode(NM_Client))
	{
		// 클라이언트에서 로컬 정리 작업
		SessionInterface->DestroySession(playerName);
		ClientLeaveSession();
	}
}

void UJS_SessionGameInstanceSubSystem::ClientLeaveSession_Implementation()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->DestroySession(PlayerName);
	}

	// 로비로 이동
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->ClientTravel("/Game/SHK/Level/HG_LobbyLevel_New", TRAVEL_Absolute);
	}
}

void UJS_SessionGameInstanceSubSystem::ServerExit()
{
	SessionInterface->DestroySession(PlayerName);
}

void UJS_SessionGameInstanceSubSystem::MySessionDestroy()
{
	PRINTLOG(TEXT("DESTROY!!!!!!!!!"));
	SessionInterface->DestroySession(PlayerName);
}

