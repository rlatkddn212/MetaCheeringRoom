// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_StoreWidget.h"
#include "Components/TextBlock.h"

void UHG_StoreWidget::SetPointText(int32 Value)
{
	TB_Point->SetText(FText::FromString(FString::Printf(TEXT("%d Point"),Value)));
}
