// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_SummaryResultWidget.h"
#include "JS_Screen.h"
#include "JS_NetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

void UJS_SummaryResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Screen = Cast<AJS_Screen>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_Screen::StaticClass()));
	if (Screen && Screen->NetComp)
	{
		Screen->NetComp->OnSummarySignatureCompleteDelegate.AddDynamic(this, &UJS_SummaryResultWidget::ShowSummaryData);
	}
}

void UJS_SummaryResultWidget::ShowSummaryData(const FString& Result)
{
	TEXT_Summary->SetText(FText::FromString(Result));
	PlayAnimation(SummaryActiveAnim);
}
