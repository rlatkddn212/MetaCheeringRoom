// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JS_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API AJS_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AJS_PlayerController();

	virtual void BeginPlay() override;

    UPROPERTY()
    class AJS_TotoActor* TotoActor;
	UFUNCTION(Server, Reliable)
	void ServerHandleBettingToTo(int32 point, int32 select, const FString& UserId);

	UPROPERTY()
    class AJS_Screen* ScreenActor;
	UFUNCTION(Server, Reliable)
	void ServerHandleVideoPlay();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UJS_ExitWidget> ExitWidgetFactory;
	
	UPROPERTY(BlueprintReadOnly)
	class UJS_ExitWidget* ExitWidget;

	UPROPERTY()
    class AJS_Screen* MyScreenActor;

	UFUNCTION(Client, Reliable)
	void ClientHandleVideoPlay(const FString& URL, const FString& streamID, int32 segmentNumber);
};
