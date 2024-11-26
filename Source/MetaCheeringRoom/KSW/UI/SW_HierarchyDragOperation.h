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

	// 드래그 시작한 위젯과 같은 위젯인지 확인
	// 드래그 시작한 위젯과 같은 위젯이라면 OnDrop이 아니라 NativeOnDragCancelled 이 호출 되므로
	// 계층 구조가 풀리는 문제가 있다. 그래서 같은 위젯일 경우 따로 처리해준다.
	bool bSameObject = false;
};
