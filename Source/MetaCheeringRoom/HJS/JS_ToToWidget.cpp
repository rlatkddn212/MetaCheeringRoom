 // Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_ToToWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Animation/WidgetAnimation.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "../MetaCheeringRoom.h"
#include "Components/BackgroundBlur.h"
#include "Kismet/GameplayStatics.h"
#include "HJS/JS_TotoActor.h"
#include "../SHK/HG_Player.h"
#include "Components/EditableTextBox.h"
#include "../SHK/HG_PlayerGoodsComponent.h"
#include "JS_EasingFunctionLib.h"


void UJS_ToToWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_ToToQuit->OnClicked.AddDynamic(this, &UJS_ToToWidget::OnClickQuitBtn);

	BTN_Betting1->OnClicked.AddDynamic(this, &UJS_ToToWidget::OnClickBetting1Btn);
	BTN_Betting2->OnClicked.AddDynamic(this, &UJS_ToToWidget::OnClickBetting2Btn);
	BTN_BettingWeight->OnClicked.AddDynamic(this, &UJS_ToToWidget::OnClickBettingWeightBtn);
	ET_BettingWeight1->OnTextChanged.AddDynamic(this, &UJS_ToToWidget::OnBetting1Changed);
	ET_BettingWeight2->OnTextChanged.AddDynamic(this, &UJS_ToToWidget::OnBetting2Changed);
	BTN_WeightBetting1->OnClicked.AddDynamic(this, &UJS_ToToWidget::OnClickWeightBtn1);
	BTN_WeightBetting2->OnClicked.AddDynamic(this, &UJS_ToToWidget::OnClickWeightBtn2);
}

void UJS_ToToWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry,DeltaTime);
	OnAnimation(DeltaTime);
}

void UJS_ToToWidget::OnClickBetting1Btn()
{
	int32 BettingPoint = FCString::Atoi(*TEXT_Betting1->GetText().ToString());
	// 베팅하기
	AJS_TotoActor* TotoActor = Cast<AJS_TotoActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_TotoActor::StaticClass()));
	
	// 플레이어가 이만한 포인트가 있는 지 체크하는 부분
	AHG_Player* Player = Cast<AHG_Player>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (Player)
	{
		UHG_PlayerGoodsComponent* PG = Player->GoodsComp;
		if (PG)
		{
			if (BettingPoint > PG->GetGold())
			{
				return;
			}
			PG->AddGold(-BettingPoint);
		}
	}
	TotoActor->BettingToto(BettingPoint,1);
	BTN_Betting2->SetIsEnabled(false);
	BTN_WeightBetting2->SetIsEnabled(false);
}

void UJS_ToToWidget::OnClickBetting2Btn()
{
	// 플레이어가 이만한 포인트가 있는 지 체크하는 부분
	int32 BettingPoint = FCString::Atoi(*TEXT_Betting2->GetText().ToString());
	// 베팅하기
	AJS_TotoActor* TotoActor = Cast<AJS_TotoActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_TotoActor::StaticClass()));

	AHG_Player* Player = Cast<AHG_Player>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (Player)
	{
		UHG_PlayerGoodsComponent* PG = Player->GoodsComp;
		if (PG)
		{
			if (BettingPoint > PG->GetGold())
			{
				return;
			}
			PG->AddGold(-BettingPoint);
		}
	}
	TotoActor->BettingToto(BettingPoint, 2);
	BTN_Betting1->SetIsEnabled(false);
	BTN_WeightBetting1->SetIsEnabled(false);
}

void UJS_ToToWidget::OnClickQuitBtn()
{
	BD_ToToMain->SetVisibility(ESlateVisibility::Hidden);
	BB_ToToMain->SetVisibility(ESlateVisibility::Hidden);
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

void UJS_ToToWidget::SetTotoInfo(const FString& totoName, const FString& select1, const FString& select2, int32 totalSelect1 /*= 0*/, int32 totalSelect2 /*= 0*/, int32 totalBettor1 /*= 0*/, int32 totalBettor2 /*= 0*/, float totalOdds1 /*= 1.f*/, float totalOdds2 /*= 1.f*/)
{
	// 토토 제목
	TEXT_ToToName->SetText(FText::FromString(totoName));
	// 토토 선택지
	TEXT_TimeLimit->SetVisibility(ESlateVisibility::Visible);
	FString Select1Text = select1.TrimStartAndEnd();
	TEXT_Select1->SetText(FText::FromString(Select1Text));
	FString Select2Text = select2.TrimStartAndEnd();
	TEXT_Select2->SetText(FText::FromString(Select2Text));
	// 총 포인트
	FString Point1Text = FString::Printf(TEXT("%d"), totalSelect1);
	TEXT_PointSelect1->SetText(FText::FromString(Point1Text));
	FString Point2Text = FString::Printf(TEXT("%d"), totalSelect2);
	TEXT_PointSelect2->SetText(FText::FromString(Point2Text));
	// 포인트 비율
	float Total = (float)totalSelect1 + totalSelect2;
	if (Total == 0.f)
	{
		Total = 1.f;
	}
	float Percent1 = (totalSelect1 / Total) * 100.f;
	float Percent2 = (totalSelect2 / Total) * 100.f;
	int32 Percent1Int = FMath::RoundToInt32(Percent1);
	int32 Percent2Int = FMath::RoundToInt32(Percent2);
	FString Percent1Text = FString::Printf(TEXT("%d%%"), Percent1Int);
	TEXT_Percent1->SetText(FText::FromString(Percent1Text));
	FString Percent2Text = FString::Printf(TEXT("%d%%"), Percent2Int);
	TEXT_Percent2->SetText(FText::FromString(Percent2Text));

	PB_Percent1->SetPercent(Percent1/100.f);
	PB_Percent2->SetPercent(Percent2/100.f);

	FString Odd1Text = FString::Printf(TEXT("%.2f"), totalOdds1);	
	TEXT_OddsSelect1->SetText(FText::FromString(Odd1Text));
	FString Odd2Text = FString::Printf(TEXT("%.2f"), totalOdds2);
	TEXT_OddsSelect2->SetText(FText::FromString(Odd2Text));
	
	FString BettorCount1Text = FString::Printf(TEXT("%d"), totalBettor1);
	TEXT_BettorCount1->SetText(FText::FromString(BettorCount1Text));
	FString BettorCount2Text = FString::Printf(TEXT("%d"), totalBettor2);
	TEXT_BettorCount2->SetText(FText::FromString(BettorCount2Text));

}

void UJS_ToToWidget::AlarmToto(FString TostMsg)
{
	// Text로 Text를 설정해주고
	TEXT_ToToTost->SetText(FText::FromString(TostMsg));
	// Tost Visible 처리 하고
	BD_Tost->SetVisibility(ESlateVisibility::Visible);
	// UI Animation 재생하기
	PlayAnimation(TostAnim);
}

void UJS_ToToWidget::SetTimerText(FString TimerText)
{
	TEXT_TimeLimit->SetText(FText::FromString(TimerText));
}

void UJS_ToToWidget::SetBettingStopUI()
{
	WS_Betting->SetActiveWidgetIndex(2);
}

void UJS_ToToWidget::ToToInitSetting()
{
	BD_ToToMain->SetVisibility(ESlateVisibility::Visible);
	BB_ToToMain->SetVisibility(ESlateVisibility::Visible);
}


void UJS_ToToWidget::InitWidget()
{
	TEXT_ToToName->SetText(FText::FromString(TEXT("")));
	TEXT_TimeLimit->SetText(FText::FromString(TEXT("시작 대기중입니다.")));
	WS_Betting->SetActiveWidgetIndex(0);
	BTN_Betting1->SetIsEnabled(true);
	BTN_Betting2->SetIsEnabled(true);
}

void UJS_ToToWidget::OnClickBettingWeightBtn()
{
	WS_Betting->SetActiveWidgetIndex(1);
}

void UJS_ToToWidget::OnBetting1Changed(const FText& Text)
{

	if (!Text.ToString().IsNumeric())
	{
		ET_BettingWeight1->SetText(FText::FromString(TEXT("")));
		return;
	}

	AHG_Player* player = Cast<AHG_Player>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	int32 PlayerGold = 0;
	if (player)
	{
		UHG_PlayerGoodsComponent* goodsComp = player->GoodsComp;
		if (goodsComp)
		{
			PlayerGold = goodsComp->GetGold();
		}
	}
	FString GoldText;
	if (FCString::Atoi(*Text.ToString()) >= PlayerGold)
	{
		GoldText = FString::Printf(TEXT("%d"), PlayerGold);
	}
	else
	{
		GoldText = Text.ToString();
	}
	ET_BettingWeight1->SetText(FText::FromString(GoldText));
	TEXT_Betting1->SetText(FText::FromString(GoldText));
}

void UJS_ToToWidget::OnBetting2Changed(const FText& Text)
{
	if (!Text.ToString().IsNumeric())
	{
		ET_BettingWeight2->SetText(FText::FromString(TEXT("")));
		return;
	}
	AHG_Player* player = Cast<AHG_Player>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	int32 PlayerGold = 0;
	if (player)
	{
		UHG_PlayerGoodsComponent* goodsComp = player->GoodsComp;
		if (goodsComp)
		{
			PlayerGold = goodsComp->GetGold();
		}
	}
	FString GoldText;
	if (FCString::Atoi(*Text.ToString()) >= PlayerGold)
	{
		GoldText = FString::Printf(TEXT("%d"), PlayerGold);
	}
	else
	{
		GoldText = Text.ToString();
	}
	ET_BettingWeight2->SetText(FText::FromString(GoldText));
	TEXT_Betting2->SetText(FText::FromString(GoldText));
}

void UJS_ToToWidget::OnClickWeightBtn1()
{
	OnClickBetting1Btn();
	WS_Betting->SetActiveWidgetIndex(0);
}

void UJS_ToToWidget::OnClickWeightBtn2()
{
	OnClickBetting2Btn();
	WS_Betting->SetActiveWidgetIndex(0);
}

void UJS_ToToWidget::PlayShowAnimation()
{
	AnimationAlpha = 0.0f;  
	bIsAnimating = true;  
	StartPosition = BD_ToToMain->GetRenderTransform().Translation.Y - TargetOffset; 
}

void UJS_ToToWidget::OnAnimation(float DeltaTime)
{
	if (bIsAnimating)
	{
		// Alpha 값 증가시키기
		AnimationAlpha += DeltaTime / AnimationDuration;
		AnimationAlpha = FMath::Clamp(AnimationAlpha, 0.0f, 1.f);  // 0~1 사이로 제한

		// 이징 함수 호출 (UI가 나타날 때 ElasticEaseOut 적용)
		float NewPosition = UJS_EasingFunctionLib::ElasticEaseOut(AnimationAlpha);
		// NewPosition 값을 UI 위치에 적용 (예: Y 위치를 NewPosition으로 설정)
		BD_ToToMain->SetRenderTranslation(FVector2D(0, StartPosition + NewPosition * TargetOffset));

		// 애니메이션이 끝나면 bIsAnimating 비활성화
		if (AnimationAlpha >= 1.0f)
		{
			bIsAnimating = false;
		}
	}
}
