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
	
	UFUNCTION()
	void AnimNotify_HDEnd();
	UFUNCTION()
	void AnimNotify_PrayEnd();
	
	UFUNCTION()
	void AnimNotify_MLTEnd(); 

	UFUNCTION()
	void AnimNotify_CheeringEnd();
	
	UFUNCTION()
	void AnimNotify_CartWheelEnd();
	
	UFUNCTION()
	void AnimNotify_ClappingEnd();
	
	UFUNCTION()
	void AnimNotify_DefeatEnd();
	
	UFUNCTION()
	void AnimNotify_FlyKickEnd();
	
	UFUNCTION()
	void AnimNotify_WavingEnd();
	
	UFUNCTION()
	void AnimNotify_PushUpEnd();
	UFUNCTION()
	void AnimNotify_KTTGEnd();

	class AHG_Player* Owner;

	UFUNCTION()
	void PlaySelectedMontage(UAnimMontage* p_Montage);
};
