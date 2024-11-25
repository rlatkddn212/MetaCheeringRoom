// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SW_HierarchyDragOperation.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_HierarchyDragOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	FString ItemName;
	UPROPERTY()
	class ASW_CreatorObject* CreatorObject;

	// �巡�� ������ ������ ���� �������� Ȯ��
	// �巡�� ������ ������ ���� �����̶�� OnDrop�� �ƴ϶� NativeOnDragCancelled �� ȣ�� �ǹǷ�
	// ���� ������ Ǯ���� ������ �ִ�. �׷��� ���� ������ ��� ���� ó�����ش�.
	bool bSameObject = false;
};
