// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VideoNode.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UVideoNode : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWIdget))
	class UBorder* IMG_Thumbnail;

	UPROPERTY(meta=(BindWIdget))
	class UTextBlock* TEXT_Time;
	UPROPERTY(meta=(BindWIdget))
	class UTextBlock* TEXT_Title;
	UPROPERTY(meta=(BindWIdget))
	class UTextBlock* TEXT_Owner;
	UPROPERTY(meta=(BindWIdget))
	class UButton* BTN_Play;

	FString PlayURL;

	bool bLive = true;

	UFUNCTION()
	void PlayStream();

};
