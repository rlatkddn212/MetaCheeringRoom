// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_CreatorMenuWidget.h"
#include "Components/Button.h"
#include "../CreatorMapSubsystem.h"
#include "../CreatorStorageSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Border.h"
#include "HJS/JS_SessionGameInstanceSubSystem.h"

void USW_CreatorMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SaveButton->OnClicked.AddDynamic(this, &USW_CreatorMenuWidget::SaveCreatorMap);
	QuitButton->OnClicked.AddDynamic(this, &USW_CreatorMenuWidget::Quit);
}

void USW_CreatorMenuWidget::SaveCreatorMap()
{
	UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
	FString str = system->SaveCreatorMapToJson();
	FString MapName = system->GetMapName();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *str);

	UCreatorStorageSubsystem * storage = GetGameInstance()->GetSubsystem<UCreatorStorageSubsystem>();
	storage->SaveCreatorMap(str, MapName);
	BD_Toast->SetVisibility(ESlateVisibility::Visible);
	// UI Animation 재생하기
	PlayAnimation(ToastAnim);
}

void USW_CreatorMenuWidget::Quit()
{
	// 탈출 
	UJS_SessionGameInstanceSubSystem* si = GetGameInstance()->GetSubsystem<UJS_SessionGameInstanceSubSystem>();
	si->ExitSession();
	// UGameplayStatics::OpenLevel(GetWorld(), "HG_LobbyLevel_New", true);
}
