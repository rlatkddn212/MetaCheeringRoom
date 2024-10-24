// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HG_ItemBase.h"
#include "HG_DisplayStandBase.generated.h"

class UHG_ItemPurchaseWidget;

UCLASS()
class METACHEERINGROOM_API AHG_DisplayStandBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHG_DisplayStandBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;

	FItemData ItemData;

	void InitItemData();

	class UHG_GameInstance* GI;

	void SetItemData(FItemData ItemValue);

	void Detected(bool Value, APawn* p_DetectActor);

	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* InteractionWidget;

	bool bToggle = false;

	bool bOnDetect = false;

	UPROPERTY(EditDefaultsOnly)
	APawn* DetectActor;

	bool bDetected = false;
};
