// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/ToToMakeWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "JS_TotoActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SpinBox.h"
#include "Components/WidgetSwitcher.h"
#include "../SHK/HG_Player.h"
#include "JS_EasingFunctionLib.h"
#include "Components/Border.h"
void UToToMakeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// EditBox 처리
	ET_BettingName->OnTextChanged.AddDynamic(this,&UToToMakeWidget::OnBettingNameChanged);
	ET_Select1->OnTextChanged.AddDynamic(this,&UToToMakeWidget::OnSelect1Changed);
	ET_Select2->OnTextChanged.AddDynamic(this,&UToToMakeWidget::OnSelect2Changed);

	// Button 처리
	BTN_Cancel->OnClicked.AddDynamic(this, &UToToMakeWidget::OnClickCancelBtn);
	BTN_CancelResult->OnClicked.AddDynamic(this, &UToToMakeWidget::OnClickCancelBtn);
	BTN_StartBetting->OnClicked.AddDynamic(this, &UToToMakeWidget::OnClickStartBtn);
	BTN_ResultSelect1->OnClicked.AddDynamic(this, &UToToMakeWidget::OnClickResult1Btn);
	BTN_ResultSelect2->OnClicked.AddDynamic(this, &UToToMakeWidget::OnClickResult2Btn);
	BTN_Adjust->OnClicked.AddDynamic(this, &UToToMakeWidget::OnClickAdjustBtn);

}

void UToToMakeWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry,DeltaTime);
	OnAnimation(DeltaTime);
}

void UToToMakeWidget::SetWidgetSwitcher(int32 value)
{
	WS_MakeToTo->SetActiveWidgetIndex(value);
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
		FString LimitText = FString::Printf(TEXT("%d/%d"), NewTextLength, MAX_SELECT_LEN);
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
		FString LimitText = FString::Printf(TEXT("%d/%d"), NewTextLength, MAX_SELECT_LEN);
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
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	PC->SetShowMouseCursor(false);
	PC->SetInputMode(FInputModeGameOnly());
	AHG_Player* Player = Cast<AHG_Player>(PC->GetCharacter());
	if (Player)
	{
		Player->Direction = FVector::ZeroVector;
		Player->bCanMove = true;
	}
}

void UToToMakeWidget::OnClickStartBtn()
{
	// 1. 저장되어있던 정보를 UI액터에 전송하기
	TEXT_BettingName->SetText(ET_BettingName->GetText());
	TEXT_Result1->SetText(ET_Select1->GetText());
	TEXT_Result2->SetText(ET_Select2->GetText());
	AJS_TotoActor* TotoActor = Cast<AJS_TotoActor>(UGameplayStatics::GetActorOfClass(GetWorld(),AJS_TotoActor::StaticClass()));
	int32 Second = SPB_TimeLimit->GetValue()*60 - 50;
	TotoActor->MakeToto(ET_BettingName->GetText().ToString(),ET_Select1->GetText().ToString(),ET_Select2->GetText().ToString(), Second);
	// 2. 창 닫기
	SetVisibility(ESlateVisibility::Hidden);
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	PC->SetShowMouseCursor(false);
	PC->SetInputMode(FInputModeGameOnly());
	bOpen = false;
	AHG_Player* Player = Cast<AHG_Player>(PC->GetCharacter());
	if (Player)
	{
		Player->Direction = FVector::ZeroVector;
		Player->bCanMove = true;
	}
}

void UToToMakeWidget::OnClickAdjustBtn()
{
	// ToToActor에 Result 결과 뿌려주기
	AJS_TotoActor* TotoActor = Cast<AJS_TotoActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_TotoActor::StaticClass()));
	TotoActor->MulticastAlarmToto(TEXT("예측 결과가 나왔습니다!!"));
	TotoActor->AdjustPoint(SelectResult);
	TotoActor->MulticastInitToto(false);
	// 처음 상태로 되돌리기
	InitMakeWidget();
}

void UToToMakeWidget::OnClickResult1Btn()
{
	SelectResult = 1;
	BTN_ResultSelect1->SetBackgroundColor(FLinearColor(0.207f, 0.047f, 0.71f, 1.0f));
	BTN_ResultSelect2->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
}

void UToToMakeWidget::OnClickResult2Btn()
{
	SelectResult = 2;
	BTN_ResultSelect2->SetBackgroundColor(FLinearColor(0.207f, 0.047f, 0.71f, 1.0f));
	BTN_ResultSelect1->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
}

void UToToMakeWidget::InitMakeWidget()
{
	SelectResult = -1;
	BTN_ResultSelect1->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
	BTN_ResultSelect2->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
	ET_BettingName->SetText(FText::FromString(TEXT("")));
	ET_Select1->SetText(FText::FromString(TEXT("")));
	ET_Select2->SetText(FText::FromString(TEXT("")));
	SPB_TimeLimit->SetValue(1);
	WS_MakeToTo->SetActiveWidgetIndex(0);
	FString LimitText = FString::Printf(TEXT("%d/%d"), 0, MAX_TEXT_LEN);
	TEXT_BettingLimit->SetText(FText::FromString(LimitText));
	LimitText = FString::Printf(TEXT("%d/%d"), 0, MAX_SELECT_LEN);
	TEXT_SelectLimit1->SetText(FText::FromString(LimitText));
	LimitText = FString::Printf(TEXT("%d/%d"), 0, MAX_SELECT_LEN);
	TEXT_SelectLimit2->SetText(FText::FromString(LimitText));
	SetVisibility(ESlateVisibility::Hidden);
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	PC->SetShowMouseCursor(false);
	PC->SetInputMode(FInputModeGameOnly());
	AHG_Player* Player = Cast<AHG_Player>(PC->GetCharacter());
	if (Player)
	{
		Player->Direction = FVector::ZeroVector;
		Player->bCanMove = true;
	}
}

void UToToMakeWidget::PlayShowAnimation()
{
	AnimationAlpha = 0.0f;   // Alpha 초기화
	bIsAnimating = true;     // 애니메이션 시작
	StartPosition1 = BD_MakeToto->GetRenderTransform().Translation.Y - TargetOffset; // 초기 위치 설정
	StartPosition2 = BD_ResultToto->GetRenderTransform().Translation.Y - TargetOffset; // 초기 위치 설정
}

void UToToMakeWidget::OnAnimation(float DeltaTime)
{
	if (bIsAnimating)
	{
		// Alpha 값 증가시키기
		AnimationAlpha += DeltaTime / AnimationDuration;
		AnimationAlpha = FMath::Clamp(AnimationAlpha, 0.0f, 1.f);  // 0~1 사이로 제한

		// 이징 함수 호출 (UI가 나타날 때 ElasticEaseOut 적용)
		float NewPosition = UJS_EasingFunctionLib::ElasticEaseOut(AnimationAlpha);
		// NewPosition 값을 UI 위치에 적용 (예: Y 위치를 NewPosition으로 설정)
		BD_MakeToto->SetRenderTranslation(FVector2D(0, StartPosition1 + NewPosition * TargetOffset));
		BD_ResultToto->SetRenderTranslation(FVector2D(0, StartPosition2 + NewPosition * TargetOffset));

		// 애니메이션이 끝나면 bIsAnimating 비활성화
		if (AnimationAlpha >= 1.0f)
		{
			bIsAnimating = false;
		}
	}
}
