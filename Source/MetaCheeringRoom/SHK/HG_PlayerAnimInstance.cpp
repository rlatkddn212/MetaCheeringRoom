// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_PlayerAnimInstance.h"
#include "HG_Player.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

void UHG_PlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<AHG_Player>(GetOwningActor());
}

void UHG_PlayerAnimInstance::AnimNotify_TwerkEnd()
{
	EndAnimSetting();
}

void UHG_PlayerAnimInstance::AnimNotify_HDEnd()
{
	EndAnimSetting();
}

void UHG_PlayerAnimInstance::AnimNotify_PrayEnd()
{
	EndAnimSetting();
}

void UHG_PlayerAnimInstance::AnimNotify_MLTEnd()
{
	EndAnimSetting();
}

void UHG_PlayerAnimInstance::AnimNotify_CheeringEnd()
{
	EndAnimSetting();
}

void UHG_PlayerAnimInstance::AnimNotify_CartWheelEnd()
{
	EndAnimSetting();
}

void UHG_PlayerAnimInstance::AnimNotify_ClappingEnd()
{
	EndAnimSetting();
}

void UHG_PlayerAnimInstance::AnimNotify_DefeatEnd()
{
	EndAnimSetting();
}

void UHG_PlayerAnimInstance::AnimNotify_FlyKickEnd()
{
	EndAnimSetting();
}

void UHG_PlayerAnimInstance::AnimNotify_WavingEnd()
{
	EndAnimSetting();
}

void UHG_PlayerAnimInstance::AnimNotify_PushUpEnd()
{
	EndAnimSetting();
}

void UHG_PlayerAnimInstance::AnimNotify_KTTGEnd()
{
	EndAnimSetting();
}


void UHG_PlayerAnimInstance::PlaySelectedMontage(UAnimMontage* p_Montage)
{
	if (p_Montage)
	{
		Montage_Play(p_Montage);

		Owner->bCanMove = false;
		Owner->bIsInEmotion = true;
		Owner->CameraComp->FieldOfView = 120.0f;
	}
}

void UHG_PlayerAnimInstance::EndAnimSetting()
{
	if (Owner)
	{
		Owner->bCanMove = true;
		Owner->bIsInEmotion = false;
		Owner->CameraComp->FieldOfView = 90.0f;
		auto* pc = Cast<APlayerController>(Owner->Controller);
		if (pc)
		{
			pc->bShowMouseCursor = false;
		}
	}
}
