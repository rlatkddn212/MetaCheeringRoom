// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_PlayerAnimInstance.h"
#include "HG_Player.h"
#include "Camera/CameraComponent.h"

void UHG_PlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<AHG_Player>(GetOwningActor());
}
void UHG_PlayerAnimInstance::PlayTwerkEmotionMontage()
{
	if (Owner && TwerkMontage)
	{
		Montage_Play(TwerkMontage);

		Owner->bCanMove = false;
		Owner->CameraComp->FieldOfView = 120.0f;
	}
}
void UHG_PlayerAnimInstance::AnimNotify_TwerkEnd()
{
	if (Owner)
	{
		Owner->bCanMove = true;
		Owner->CameraComp->FieldOfView = 90.0f;
	}
}

void UHG_PlayerAnimInstance::AnimNotify_HDEnd()
{
	if (Owner)
	{
		Owner->bCanMove = true;
		Owner->CameraComp->FieldOfView = 90.0f;
	}
}

void UHG_PlayerAnimInstance::PlaySelectedMontage(UAnimMontage* p_Montage)
{
	if (p_Montage)
	{
		Montage_Play(p_Montage);

		if (Owner->IsLocallyControlled())
		{
			Owner->bCanMove = false;
			Owner->CameraComp->FieldOfView = 120.0f;
		}
	}
}
