// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/ToToMakeWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
void UToToMakeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// EditBox 처리
	ET_BettingName->OnTextChanged.AddDynamic(this,&UToToMakeWidget::OnBettingNameChanged);
	ET_Select1->OnTextChanged.AddDynamic(this,&UToToMakeWidget::OnSelect1Changed);
	ET_Select2->OnTextChanged.AddDynamic(this,&UToToMakeWidget::OnSelect2Changed);

	// Button 처리
	BTN_Cancel->OnClicked.AddDynamic(this, &UToToMakeWidget::OnClickCancelBtn);
	BTN_StartBetting->OnClicked.AddDynamic(this, &UToToMakeWidget::OnClickStartBtn);
	
}

void UToToMakeWidget::OnSelect1Changed(const FText& Text)
{
	FString NewText = Text.ToString();
	int32 NewTextLength = NewText.Len();
	if (NewTextLength > MAX_TEXT_LEN)
	{
		NewText = NewText.Left(MAX_TEXT_LEN);
		ET_Select1->SetText(FText::FromString(NewText));
	}
	else
	{
		FString LimitText = FString::Printf(TEXT("%d/%d"), NewTextLength, MAX_TEXT_LEN);
		// 변하는 동안에는 그냥 UI에서만 처리하기
		TEXT_SelectLimit1->SetText(FText::FromString(LimitText));
	}

	CheckCanMakeToTo();
}

void UToToMakeWidget::OnSelect2Changed(const FText& Text)
{
	FString NewText = Text.ToString();
	int32 NewTextLength = NewText.Len();
	if (NewTextLength > MAX_TEXT_LEN)
	{
		NewText = NewText.Left(MAX_TEXT_LEN);
		ET_Select2->SetText(FText::FromString(NewText));
	}
	else
	{
		FString LimitText = FString::Printf(TEXT("%d/%d"), NewTextLength, MAX_TEXT_LEN);
		// 변하는 동안에는 그냥 UI에서만 처리하기
		TEXT_SelectLimit2->SetText(FText::FromString(LimitText));
	}

	CheckCanMakeToTo();
}

void UToToMakeWidget::OnBettingNameChanged(const FText& Text)
{
	FString NewText = Text.ToString();
	int32 NewTextLength = NewText.Len();
	if (NewTextLength > MAX_TEXT_LEN)
	{
		NewText = NewText.Left(MAX_TEXT_LEN);
		ET_BettingName->SetText(FText::FromString(NewText));
	}
	else
	{
		FString LimitText = FString::Printf(TEXT("%d/%d"),NewTextLength,MAX_TEXT_LEN);
		// 변하는 동안에는 그냥 UI에서만 처리하기
		TEXT_BettingLimit->SetText(FText::FromString(LimitText));
	}

	CheckCanMakeToTo();
}

void UToToMakeWidget::CheckCanMakeToTo()
{
	// 제목 설정
	FString CheckStr = ET_BettingName->GetText().ToString().TrimStartAndEnd();
	if (CheckStr.IsEmpty())
	{
		BTN_StartBetting->SetIsEnabled(false);
		return;
	}

	CheckStr = ET_Select1->GetText().ToString().TrimStartAndEnd();
	if (CheckStr.IsEmpty())
	{
		BTN_StartBetting->SetIsEnabled(false);
		return;
	}

	CheckStr = ET_Select2->GetText().ToString().TrimStartAndEnd();
	if (CheckStr.IsEmpty())
	{
		BTN_StartBetting->SetIsEnabled(false);
		return;
	}

	BTN_StartBetting->SetIsEnabled(true);
}

void UToToMakeWidget::OnClickCancelBtn()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UToToMakeWidget::OnClickStartBtn()
{
	// 1. 저장되어있던 정보를 UI액터에 전송하기
	
	// 2. 창 닫기
	SetVisibility(ESlateVisibility::Hidden);
}