// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_LoadActor.h"
#include "../KSW/CreatorMapSubsystem.h"
#include "JS_ChattingWidget.h"
// Sets default values
AJS_LoadActor::AJS_LoadActor()
{
}

// Called when the game starts or when spawned
void AJS_LoadActor::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if(PC->HasAuthority())
	{
		UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
		system->LoadMap();
	}

	if (ChatWidgetFactory)
	{
		ChatWidget = CreateWidget<UJS_ChattingWidget>(GetWorld(), ChatWidgetFactory);
		if (ChatWidget)
		{
			ChatWidget->AddToViewport(30);
		}
	}
}

