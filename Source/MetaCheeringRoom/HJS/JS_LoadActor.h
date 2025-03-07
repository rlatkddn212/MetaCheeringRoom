// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_LoadActor.generated.h"

UCLASS()
class METACHEERINGROOM_API AJS_LoadActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_LoadActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UJS_ChattingWidget> ChatWidgetFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UJS_ChattingWidget* ChatWidget;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAddChat(const FString& id, const FText& text, bool bAuto);

	UFUNCTION(NetMulticast,Reliable,BlueprintCallable)
	void MulticastAutoChatOpen();

};
