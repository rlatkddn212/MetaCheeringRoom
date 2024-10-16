// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SW_ObjectDragOperation.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_ObjectDragOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
	//
public:
	UPROPERTY(BlueprintReadWrite)
	FString ItemName;

};
