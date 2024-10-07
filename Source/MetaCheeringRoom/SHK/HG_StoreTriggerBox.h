// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HG_StoreTriggerBox.generated.h"

UCLASS()
class METACHEERINGROOM_API AHG_StoreTriggerBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHG_StoreTriggerBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UHG_StoreWidget> StoreWidgetFactory;

	UPROPERTY() 
	class UHG_StoreWidget* StoreWidget;

	UFUNCTION() 
	void CreateStoreWidget();

	UFUNCTION()
	void ByInteraction();

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxCollisionComp;

	bool bToggle = false;
};
