// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_CreatorMenuWidget.h"
#include "Components/Button.h"
#include "../CreatorMapSubsystem.h"
#include "../CreatorStorageSubsystem.h"

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
	UE_LOG(LogTemp, Warning, TEXT("%s"), *str);

	UCreatorStorageSubsystem * storage = GetGameInstance()->GetSubsystem<UCreatorStorageSubsystem>();
	
	FString path = FPaths::ProjectContentDir() + TEXT("CreatorMap.json");
	storage->SaveCreatorMap(path, str);
}

void USW_CreatorMenuWidget::Quit()
{

}
