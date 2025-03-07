// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_SumarryRequestWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "JS_Screen.h"
#include "Components/ComboBoxString.h"
#include "JS_NetComponent.h"
#include "../SHK/HG_Player.h"
#include "JS_EasingFunctionLib.h"
#include "Components/Border.h"

void UJS_SumarryRequestWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_Summary->OnClicked.AddDynamic(this,&UJS_SumarryRequestWidget::OnClickSummaryBtn);
	BTN_Cancel->OnClicked.AddDynamic(this, &UJS_SumarryRequestWidget::OnClickCancelBtn);

	Screen = Cast<AJS_Screen>(UGameplayStatics::GetActorOfClass(GetWorld(),AJS_Screen::StaticClass()));

}

void UJS_SumarryRequestWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry,DeltaTime);
	OnAnimation(DeltaTime);
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

void UJS_SumarryRequestWidget::PlayShowAnimation()
{
	AnimationAlpha = 0.0f;
	bIsAnimating = true;
	StartPosition = BD_Summary->GetRenderTransform().Translation.Y - TargetOffset;
}

void UJS_SumarryRequestWidget::OnAnimation(float DeltaTime)
{
	if (bIsAnimating)
	{
		// Alpha 값 증가시키기
		AnimationAlpha += DeltaTime / AnimationDuration;
		AnimationAlpha = FMath::Clamp(AnimationAlpha, 0.0f, 1.f);  // 0~1 사이로 제한

		// 이징 함수 호출 (UI가 나타날 때 ElasticEaseOut 적용)
		float NewPosition = UJS_EasingFunctionLib::ElasticEaseOut(AnimationAlpha);
		// NewPosition 값을 UI 위치에 적용 (예: Y 위치를 NewPosition으로 설정)
		BD_Summary->SetRenderTranslation(FVector2D(0, StartPosition + NewPosition * TargetOffset));
		// 애니메이션이 끝나면 bIsAnimating 비활성화
		if (AnimationAlpha >= 1.0f)
		{
			bIsAnimating = false;
		}
	}
}
