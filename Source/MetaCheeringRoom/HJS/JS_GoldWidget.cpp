// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_GoldWidget.h"
#include "../SHK/HG_Player.h"
#include "../SHK/HG_PlayerGoodsComponent.h"
#include "Components/TextBlock.h"

void UJS_GoldWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AHG_Player* player = Cast<AHG_Player>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	if (player)
	{
		UHG_PlayerGoodsComponent* PG = player->GoodsComp;
		if (PG)
		{
			FString goldText = FString::Printf(TEXT("%d"),PG->GetGold());
			TEXT_Gold->SetText(FText::FromString(goldText));
		}
	}
}