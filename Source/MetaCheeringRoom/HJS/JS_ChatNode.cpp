// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_ChatNode.h"
#include "Components/TextBlock.h"

void UJS_ChatNode::SetRandomColorText()
{
    // 랜덤 색상 선택
    int32 RandomIndex = FMath::RandRange(0, Colors.Num() - 1);
    FColor RandomColor = Colors[RandomIndex];

    // 텍스트 색상 설정
    if (TEXT_ID)
    {
        TEXT_ID->SetColorAndOpacity(FSlateColor(RandomColor));
    }
}
