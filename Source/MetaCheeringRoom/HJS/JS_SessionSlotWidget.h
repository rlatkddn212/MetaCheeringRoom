// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JS_SessionGameInstanceSubSystem.h"
#include "JS_SessionSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UJS_SessionSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	void SetupInfo(class UJS_SessionJoinWidget* parent);



	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TEXT_RoomName;	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TEXT_HostName;	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TEXT_PlayerCount;	
	UPROPERTY(meta = (BindWidget))
	class UImage* IMG_Thumbnail;
	UPROPERTY(meta = (BindWidget))
	class UImage* IMG_HiddenRoom;
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_Join;
	int32 SessionSearchIndex;
	UPROPERTY()
	class UJS_SessionJoinWidget* Parent;

	void UpdateInfo(const FRoomInfo& info);

	UFUNCTION()
	void OnJoinButtonClicked();

};
