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
	InitDummychat();

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

void UJS_ChattingWidget::OnAutoChat()
{
	GetWorld()->GetTimerManager().SetTimer(ChatTimer,this, &UJS_ChattingWidget::DummyChat, 0.1f, false);
}

void UJS_ChattingWidget::InitDummychat()
{
	IDList.Add(TEXT("asdfqwe2"));
	IDList.Add(TEXT("상혁바라기"));
	IDList.Add(TEXT("상혁나의빛"));
	IDList.Add(TEXT("DAESANGHUK"));
	IDList.Add(TEXT("BLGOUT"));
	IDList.Add(TEXT("WORLDS"));
	IDList.Add(TEXT("캐리"));

	CommentList.Add_GetRef(TEXT("대~~~상~~~혁~~~~"));
	CommentList.Add_GetRef(TEXT("신은존재하며티원에산다"));
	CommentList.Add_GetRef(TEXT("대상혁 나의 빛."));
	CommentList.Add_GetRef(TEXT("대상혁 나."));
	CommentList.Add_GetRef(TEXT("월즈 연속 우승 드가자~~~"));
	CommentList.Add_GetRef(TEXT("이게 티원이지"));
	CommentList.Add_GetRef(TEXT("서커스단 출격  ㅋㅋ"));
	CommentList.Add_GetRef(TEXT("GG~~~~"));
	CommentList.Add_GetRef(TEXT("이건 게임 갔네"));
	CommentList.Add_GetRef(TEXT("대~~~상~~~혁~~~~"));
	CommentList.Add_GetRef(TEXT("신은존재하며티원에산다"));
	CommentList.Add_GetRef(TEXT("대상혁 나의 빛."));
	CommentList.Add_GetRef(TEXT("대상혁 나."));
};

void UJS_ChattingWidget::DummyChat()
{
	int32 RandomIndex = FMath::RandRange(0, IDList.Num() - 1);
	FString ChrrentID = IDList[RandomIndex];
	RandomIndex = FMath::RandRange(0, CommentList.Num() - 1);
	FString CurrentComment = CommentList[RandomIndex];

	AddChat(ChrrentID,FText::FromString(CurrentComment),true);
	GetWorld()->GetTimerManager().SetTimer(ChatTimer, this, &UJS_ChattingWidget::DummyChat, 0.1f, false);
}