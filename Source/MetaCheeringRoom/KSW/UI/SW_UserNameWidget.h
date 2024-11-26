// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SW_UserNameWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_UserNameWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:
	void SetUserName(const FString& UserName);

	protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UserNameTextBlock;
};
