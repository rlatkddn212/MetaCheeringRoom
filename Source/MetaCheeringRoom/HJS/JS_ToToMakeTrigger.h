// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_ToToMakeTrigger.generated.h"

UCLASS()
class METACHEERINGROOM_API AJS_ToToMakeTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_ToToMakeTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
		UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;

	UPROPERTY()
	class AJS_TotoActor* ToToActor;
	UFUNCTION()
	void ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
	void ShowTotoMakeWidget();

};
