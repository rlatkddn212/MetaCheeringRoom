// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SlotStruct.h"
#include "HG_ItemBase.h"
#include "HG_GameInstance.generated.h"

/**
 *
 */
UCLASS()
class METACHEERINGROOM_API UHG_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UHG_GameInstance();

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* ItemDataTable;

	bool IsValidItem(FString ItemName);

	FItemData FindItemData(FString ItemName);

	int32 CurrentGold = 4000;

	TArray<FSlotStruct> CurrentInventory;

	TArray<FItemData> EquipItemInfoList;

	TArray<int32> EquipSlotIndexList;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	class UAnimInstance* AnimInstance;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<class UAnimInstance> Anim;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	class USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bIsFirstAccess = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 CurIdx = 0;

	int32 Gender = 0;

	FLinearColor CurCloth = FLinearColor(1.0f,1.0f,1.0f);
	FLinearColor CurClothHem = FLinearColor(1.0f, 1.0f, 1.0f);
	FLinearColor CurHairPin = FLinearColor(1.0f, 1.0f, 1.0f);
	FLinearColor CurHair = FLinearColor(1.0f, 1.0f, 1.0f);
	FLinearColor CurEyes = FLinearColor(1.0f, 1.0f, 1.0f);
	UPROPERTY()
	UTexture2D* CurPrinting = nullptr;
};