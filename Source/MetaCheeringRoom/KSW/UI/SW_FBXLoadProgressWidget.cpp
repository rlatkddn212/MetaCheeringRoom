// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_FBXLoadProgressWidget.h"
#include "Components/ProgressBar.h"

void USW_FBXLoadProgressWidget::SetBar(float LoadPercent)
{
	LoadProgressBar->SetPercent(LoadPercent);
}
