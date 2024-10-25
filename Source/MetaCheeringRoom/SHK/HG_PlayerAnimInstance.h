// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HG_PlayerAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class METACHEERINGROOM_API UHG_PlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	class UAnimMontage* TwerkMontage;

	UFUNCTION()
	void PlayTwerkEmotionMontage();

	UFUNCTION()
	void AnimNotify_TwerkEnd();

	class AHG_Player* Owner;

	UFUNCTION()
	void PlaySelectedMontage(UAnimMontage* p_Montage);
};
