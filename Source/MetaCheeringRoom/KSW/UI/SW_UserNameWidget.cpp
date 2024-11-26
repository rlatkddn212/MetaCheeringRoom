// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_UserNameWidget.h"

#include "Components/TextBlock.h"

void USW_UserNameWidget::SetUserName(const FString& UserName)
{
	UserNameTextBlock->SetText(FText::FromString(UserName));
}
