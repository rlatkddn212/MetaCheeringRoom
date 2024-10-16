// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SW_DragObjectItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_DragObjectItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ObjectImage;
};
