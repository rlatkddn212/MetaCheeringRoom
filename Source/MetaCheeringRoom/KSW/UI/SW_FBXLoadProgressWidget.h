// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SW_FBXLoadProgressWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_FBXLoadProgressWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetBar(float LoadPercent);
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* LoadProgressBar;
};
