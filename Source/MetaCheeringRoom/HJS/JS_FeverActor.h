// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_FeverActor.generated.h"

UCLASS()
class METACHEERINGROOM_API AJS_FeverActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_FeverActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UJS_FeverWidget> FeverWidgetFactory;
	
	UPROPERTY()
	class UJS_FeverWidget* FeverWidget;

	UFUNCTION(BlueprintCallable)
	void FeverTimeStart();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastFeverTimeStart();
};
