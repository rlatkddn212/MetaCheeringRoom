// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_CreatorUserJoinWidget.h"
#include "Components/TextBlock.h"

void USW_CreatorUserJoinWidget::SetJoinUser(FString JoinUser)
{
	TEXT_JoinUser->SetText(FText::FromString(JoinUser + JoinMessage));
	PlayAnimation(ToastAnim);
}
