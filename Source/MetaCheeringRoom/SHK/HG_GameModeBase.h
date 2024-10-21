// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HG_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API AHG_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UJS_SessionJoinWidget> SessionWidgetFactory;
	UPROPERTY()
	class UJS_SessionJoinWidget* SessionWidget;

};
