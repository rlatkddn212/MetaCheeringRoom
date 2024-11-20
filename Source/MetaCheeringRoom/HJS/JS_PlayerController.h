// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JS_PlayerController.generated.h"

enum class EModifyPhase
{
	None,
	Squashing,    // 찌그러지는 중
	Holding,      // 찌그러진 상태 유지
	Recovering    // 원래대로 돌아가는 중
};

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API AJS_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AJS_PlayerController();

	virtual void BeginPlay() override;
	virtual void Tick(float Deltatime) override;

    UPROPERTY()
    class AJS_TotoActor* TotoActor;
	UFUNCTION(Server, Reliable)
	void ServerHandleBettingToTo(int32 point, int32 select, const FString& UserId);

	UPROPERTY()
    class AJS_Screen* ScreenActor;
	UFUNCTION(Server, Reliable)
	void ServerHandleVideoPlay();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UJS_ExitWidget> ExitWidgetFactory;
	
	UPROPERTY(BlueprintReadOnly)
	class UJS_ExitWidget* ExitWidget;

	UPROPERTY()
    class AJS_Screen* MyScreenActor;

	UFUNCTION(Client, Reliable)
	void ClientHandleVideoPlay(const FString& URL, const FString& streamID, int32 segmentNumber);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	FString MyUserID;

	UPROPERTY(BlueprintReadOnly)
	FString MyUserNickName;

	void PlayerModify();

	void OnPlayerModify(float DeltaTime);

	FVector OriginalScale;
	bool bAnimating = false;
    UPROPERTY(EditAnywhere, Category = "Animation")
    float AnimationDuration = 0.3f;
    UPROPERTY(EditAnywhere, Category = "Animation")
    float HoldTime = 5.0f;
    UPROPERTY(EditAnywhere, Category = "Animation")
    float Squash = 0.1f;
    UPROPERTY(EditAnywhere, Category = "Animation")
    float Stretch = 1.5f;
	float CurrentTime = 0.0f;
	EModifyPhase CurrentPhase = EModifyPhase::None;
	float StunTime = 0.0f;  // 현재 스턴 시간
	const float StunDuration = 1.5f;  // 스턴 지속시간
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AJS_StarActor> StarActorFactory;
	UPROPERTY()
    AJS_StarActor* StarActor;
	void SpawnStarActor();

	UPROPERTY(EditAnywhere)
	class USoundBase* SpringSound;

	class AHG_Player* Player;

	UPROPERTY()
	class AJS_AtkActor* AtkActor;

	UFUNCTION(Server, Reliable)
	void SetMyUserID(const FString& str);

};
