// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "JS_AtkAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UJS_AtkAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	class AJS_TotoActor* Toto;
	UPROPERTY()
	class AHG_Player* Player;

	void SetTotoActor(class AJS_TotoActor* NewActor);
	UFUNCTION()
	void AnimNotify_AtkEnd();



};
