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
	void Multicast_AddCreatingDummyObject(class ASW_CreatorObject* NewCreatingObject, const struct FCreatorObjectData& ObjectData);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DeleteObject(class ASW_CreatorObject* DeleteObject);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DetachObject(class ASW_CreatorObject* DetachObject);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AttachObject(class ASW_CreatorObject* ParentObject, class ASW_CreatorObject* AttachObject);
};
