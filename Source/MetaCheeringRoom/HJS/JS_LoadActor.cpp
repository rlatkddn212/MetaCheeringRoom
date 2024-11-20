// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_LoadActor.h"
#include "../KSW/CreatorMapSubsystem.h"
#include "JS_ChattingWidget.h"
// Sets default values
AJS_LoadActor::AJS_LoadActor()
{
	bReplicates = true;
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
		SetOwner(PC);
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

void AJS_LoadActor::MulticastAddChat_Implementation(const FString& id, const FText& text, bool bAuto)
{
	if (ChatWidget)
	{
		ChatWidget->AddChat(id, text, bAuto);
	}
}
