// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SW_CreatorGameState.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API ASW_CreatorGameState : public AGameStateBase
{
	GENERATED_BODY()
	

public:
	// 플레이어컨트롤에서 해줘도 되지만 UI 업데이트가 안되는 관계로 GameState에서 처리
	// 멀티 케스트
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AddCreatingDummyObject(class ASW_CreatorObject* NewCreatingObject);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DeleteObject(class ASW_CreatorObject* DeleteObject);
};
