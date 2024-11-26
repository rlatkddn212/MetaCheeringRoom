// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SW_CreatorUserJoinWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_CreatorUserJoinWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	void SetJoinUser(FString JoinUser);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TEXT_JoinUser;
	
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* ToastAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JoinMessage")
	FString JoinMessage;
};
