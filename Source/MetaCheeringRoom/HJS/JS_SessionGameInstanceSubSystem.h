// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineSessionInterface.h"
#include "JS_SessionGameInstanceSubSystem.generated.h"

UENUM(BlueprintType)
enum class ERoomCategory : uint8
{
	CT_ESports UMETA(DisplayName = "ESports"),
	CT_Soccer UMETA(DisplayName = "Soccer"),
	CT_Idol UMETA(DisplayName = "Idol"),
	CT_Creator UMETA(DisplayName = "Creator"),
};

USTRUCT(BlueprintType)
struct FRoomInfo {
	GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
	ERoomCategory RoomCategory = ERoomCategory::CT_Soccer;
	//방이름
	UPROPERTY(BlueprintReadOnly)
	FString RoomName = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	FString HostName = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	int32 MaxPlayerCount = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPlayerCount = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 PingMS = 0;
	int32 Index = 0;
	FString ToString()
	{
		return FString::Printf(TEXT(" %d) [%s] [%s]( %d / %d)-> %dms"), Index, *RoomName, *HostName, CurrentPlayerCount, MaxPlayerCount, PingMS);
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchSignature, const struct FRoomInfo&, info);

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UJS_SessionGameInstanceSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	IOnlineSessionPtr SessionInterface;

	//방생성
	void CreateSession(const FString& RoomName, int32 PlayerCount, const FString& Category);

	void FindOtherSessions();

	//찾을 방의 목록
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	//방 조인하기
	void JoinToSession(int32 Index);

	//방 부수기
	void DestroySession();

	//델리게이트
	void OnMyCreateSessionComplete(FName SessionName, bool Success);
	void OnMyDestroySessionComplete(FName SessionName, bool Success);
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
	void LoadServerWidgetMap();
	FString StringBase64Encode(const FString& str);
	FString StringBase64Decode(const FString& str);
	void OnMyFindSessionComplete(bool Success);
	void OnMyJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	//델리게이트 걸기
	FSearchSignature OnSearchSignatureCompleteDelegate;
	FString CategoryValue;
	FString RoomNameValue;
	FString HostNameValue;

	FName PlayerName;

	//방 퇴장 요청 -> UI에서 호출
	UFUNCTION(BlueprintCallable)
	void ExitSession();
	UFUNCTION(Server, Reliable)
	void ServerRPCExitSession();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCExitSession();
	UFUNCTION(Client, Reliable)
	void ClientLeaveSession();
	UFUNCTION(BlueprintCallable)
	void MySessionDestroy();
};
