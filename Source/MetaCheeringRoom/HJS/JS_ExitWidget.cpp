// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_ExitWidget.h"
#include "Components/Button.h"
#include "../SHK/HG_Player.h"
#include "GameFramework/PlayerController.h"
#include "JS_SessionGameInstanceSubSystem.h"
#include "../SHK/HG_GameInstance.h"
#include "Kismet/KismetSystemLibrary.h"

void UJS_ExitWidget::NativeConstruct()
{
	
	Super::NativeConstruct();

	BTN_Cancel->OnClicked.AddDynamic(this, &UJS_ExitWidget::OnClickCancelBtn);

	BTN_Quit->OnClicked.AddDynamic(this, &UJS_ExitWidget::OnClickQuitBtn);
}

void UJS_ExitWidget::OnClickQuitBtn()
{	

	// 현재 레벨 불러오기
	if (GetWorld()->GetMapName() == TEXT("HG_LobbyLevel_New"))
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
	}
	else
	{
		// 세션 Destroy 함수 불러야 함
		 UHG_GameInstance* GameIns = Cast<UHG_GameInstance>(GetWorld()->GetGameInstance());
		 if (GameIns)
		 {
			 UJS_SessionGameInstanceSubSystem* SG = GameIns->GetSubsystem<UJS_SessionGameInstanceSubSystem>();
			 SG->ExitSession();
		 }
	}	
}

void UJS_ExitWidget::OnClickCancelBtn()
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
