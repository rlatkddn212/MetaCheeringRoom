// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "HG_ItemDataTable.generated.h"

USTRUCT(BlueprintType)
struct FItemData: public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ItemValue;
};
/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UHG_ItemDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
