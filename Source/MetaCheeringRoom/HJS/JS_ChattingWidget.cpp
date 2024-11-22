// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_ChattingWidget.h"
#include "Components/EditableText.h"
#include "JS_ChatNode.h"
#include "Components/ScrollBox.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MetaCheeringRoom.h"
#include "JS_PlayerController.h"
void UJS_ChattingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ED_Chatting->OnTextChanged.AddDynamic(this,&UJS_ChattingWidget::OnChatChange);
	ED_Chatting->OnTextCommitted.AddDynamic(this, &UJS_ChattingWidget::OnCommitText);
	Btn_ChatCancel->OnClicked.AddDynamic(this,&UJS_ChattingWidget::OnClickedChatCancel);
	//GetWorld()->GetTimerManager().SetTimer(ChatTimer,[this](){
	//	AddChat(TEXT("랜덤 색깔"), TEXT("텍스트 테스트 123"), true);
	//}, 0.1f, true);
}

void UJS_ChattingWidget::OnChatChange(const FText& text)
{
	FString NewText = text.ToString();
	int32 NewTextLength = NewText.Len();
	if (NewTextLength > MAX_TEXT_LEN)
	{
		NewText = NewText.Left(MAX_TEXT_LEN);
		ED_Chatting->SetText(FText::FromString(NewText));
	}
}

void UJS_ChattingWidget::ChatInit()
{
	ED_Chatting->SetKeyboardFocus();
	BD_ChatBar->SetVisibility(ESlateVisibility::Visible);
	Btn_ChatCancel->SetVisibility(ESlateVisibility::Visible);
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	bChat = true;
	if (PC)
	{
		PC->SetShowMouseCursor(true);
		PC->SetInputMode(FInputModeUIOnly());
	}
}

void UJS_ChattingWidget::OnCommitText(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		if (Text.ToString().TrimStartAndEnd().IsEmpty())
		{
			ED_Chatting->SetKeyboardFocus();
			return;
		}
		ED_Chatting->SetText(FText::FromString(TEXT("")));
		ED_Chatting->SetKeyboardFocus();
		AJS_PlayerController* PC = Cast<AJS_PlayerController>(GetWorld()->GetFirstPlayerController());
		if (PC)
		{
			PC->ServerAddChat(PC->MyUserNickName, Text, false);
		}
	}
}

void UJS_ChattingWidget::AddChat(const FString& id, const FText& text, bool bAuto)
{
	// Node를 생성해서 ID와 TEXT를 설정하고 띄워주기
	if (ChatNodeFactory)
	{
		UJS_ChatNode* ChatNode = CreateWidget<UJS_ChatNode>(this,ChatNodeFactory);
		SB_ChatBox->AddChild(ChatNode);
		SB_ChatBox->ScrollToEnd();
		ChatNode->TEXT_ID->SetText(FText::FromString(id));
		ChatNode->TEXT_Chat->SetText(text);
		ChatNode->SetRandomColorText();
		ChatNode->PlayAnimation(ChatNode->ShowChat);
	}
}

void UJS_ChattingWidget::OnClickedChatCancel()
{
	Btn_ChatCancel->SetVisibility(ESlateVisibility::Hidden);
	BD_ChatBar->SetVisibility(ESlateVisibility::Hidden);
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	bChat = false;
	if (PC)
	{
		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
	}
}
