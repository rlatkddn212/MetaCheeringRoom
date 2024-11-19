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
	// �÷��̾���Ʈ�ѿ��� ���൵ ������ UI ������Ʈ�� �ȵǴ� ����� GameState���� ó��
	// ��Ƽ �ɽ�Ʈ
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AddCreatingDummyObject(class ASW_CreatorObject* NewCreatingObject);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DeleteObject(class ASW_CreatorObject* DeleteObject);
};
