// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_SumarryRequestWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "JS_Screen.h"
#include "Components/ComboBoxString.h"
#include "JS_NetComponent.h"
#include "../SHK/HG_Player.h"

void UJS_SumarryRequestWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_Summary->OnClicked.AddDynamic(this,&UJS_SumarryRequestWidget::OnClickSummaryBtn);
	BTN_Cancel->OnClicked.AddDynamic(this, &UJS_SumarryRequestWidget::OnClickCancelBtn);

	Screen = Cast<AJS_Screen>(UGameplayStatics::GetActorOfClass(GetWorld(),AJS_Screen::StaticClass()));

}

void UJS_SumarryRequestWidget::OnClickSummaryBtn()
{
	if (Screen)
	{
		// 콤보박스의 키값에 따른 처리
		FString TimeText = CB_Summary->GetSelectedOption();
		
		int32 Time = FCString::Atoi(*TimeText.LeftChop(1));

		// 스크린액터에 접근해 요약 요청
		Screen->RequestSummaryVOD(Time);
		// 히든 처리
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
}

void UJS_SumarryRequestWidget::OnClickCancelBtn()
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
