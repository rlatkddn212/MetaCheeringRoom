// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/VideoNode.h"
#include "Components/Button.h"
#include "JS_Screen.h"
#include "VideoWidget.h"
#include "Kismet/GameplayStatics.h"
#include "../SHK/HG_Player.h"
#include "JS_EasingFunctionLib.h"

void UVideoNode::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_Play->OnClicked.AddDynamic(this, &UVideoNode::PlayStream);
}

void UVideoNode::PlayStream()
{
	AJS_Screen* Screen = Cast<AJS_Screen>(UGameplayStatics::GetActorOfClass(GetWorld(),AJS_Screen::StaticClass()));

	if (Screen)
	{
		if (bLive == false)
		{
			Screen->PlayVOD(PlayURL);
			Screen->VideoWidget->SetVisibility(ESlateVisibility::Hidden);
			APlayerController* PC = GetWorld()->GetFirstPlayerController();
			PC->SetShowMouseCursor(false);
			PC->SetInputMode(FInputModeGameOnly());
			AHG_Player* Player = Cast<AHG_Player>(PC->GetCharacter());
			if (Player)
			{
				Player->Direction = FVector::ZeroVector;
				Player->bCanMove = true;
			}
			return;
		}

		UE_LOG(LogTemp,Warning,TEXT("URL : %s"),*PlayURL);
		Screen->RequestMediaURL(PlayURL);
		
		Screen->VideoWidget->SetVisibility(ESlateVisibility::Hidden);
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