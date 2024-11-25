// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SlotStruct.h"
#include "HG_PlayerInventoryComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class METACHEERINGROOM_API UHG_PlayerInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHG_PlayerInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly)
	int32 InventorySize;

	TArray<FSlotStruct> Inventory;

	// 인벤토리에 아이템 추가
	// Parameter
	// Item : 인벤토리에 추가할 아이템의 정보(FItemData 구조체)
	// Quantity : 인벤토리에 넣을 아이템 개수
	void AddtoInventory(FItemData Item, int32 Quantity);

	//  인벤토리에 아이템이 존재하는지 존재한다면 그 아이템의 인덱스를 반환
	// Parameter 
	// ItemName : 검사하고싶은 아이템 이름
	// Return
	// int32, 찾은 아이템의 인덱스, 없으면 -1 반환
	int32 FindSlot(FString ItemName);

	// 인벤토리에서 아이템 제거
	// Parameter
	// Item : 인벤토리에 제거할 아이템의 정보(FItemData 구조체)
	// Quantity : 인벤토리에서 제거할 아이템 개수
	void RemoveFromInventory(FItemData Item, int32 Quantity);
	
	UPROPERTY(EditDefaultsOnly)
	class UDataTable* ItemDataTable;
	UPROPERTY()
	class AHG_Player* CompOwner;
};
