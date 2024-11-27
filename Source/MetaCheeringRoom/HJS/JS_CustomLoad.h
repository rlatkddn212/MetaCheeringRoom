// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_CustomLoad.generated.h"

UCLASS()
class METACHEERINGROOM_API AJS_CustomLoad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_CustomLoad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
					  

	// 20초 뒤에 불끄기
	FTimerHandle A;
	UFUNCTION()
	void AA();
	// 30초에 영상 켜기
	FTimerHandle B;
	UFUNCTION()
	void BB();
	// 65초에 파도타기, 스파크
	FTimerHandle C;
	UFUNCTION(BlueprintImplementableEvent)
	void CC();

};
