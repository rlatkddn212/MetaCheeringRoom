// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_PopupWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "JS_JoinSessionTriggerBox.h"
#include "../SHK/HG_Player.h"

void UJS_PopupWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_1->OnClicked.AddDynamic(this, &UJS_PopupWidget::OpenRoomUI);
	BTN_2->OnClicked.AddDynamic(this, &UJS_PopupWidget::OpenRoomUI);
	BTN_3->OnClicked.AddDynamic(this, &UJS_PopupWidget::OpenRoomUI);
	BTN_PopupCancel->OnClicked.AddDynamic(this, &UJS_PopupWidget::ClosePopupUI);
}

void UJS_PopupWidget::OpenRoomUI()
{
	SetVisibility(ESlateVisibility::Hidden);
	AJS_JoinSessionTriggerBox* JoinActor = Cast<AJS_JoinSessionTriggerBox>(UGameplayStatics::GetActorOfClass(GetWorld(),AJS_JoinSessionTriggerBox::StaticClass()));

	if (JoinActor)
	{
		JoinActor->ShowJoinWidget();
	}
}

void UJS_PopupWidget::ClosePopupUI()
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

void UJS_PopupWidget::Init()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	PlayAnimation(ShowWidget);
	if (PC)
	{
		PC->SetShowMouseCursor(true);
		PC->SetInputMode(FInputModeUIOnly());
		AHG_Player* Player = Cast<AHG_Player>(PC->GetCharacter());
		if (Player)
		{
			Player->Direction = FVector::ZeroVector;
			Player->bCanMove = false;
		}
	}
}
