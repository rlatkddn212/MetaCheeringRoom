// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_GameModeBase.h"
#include "../HJS/JS_SessionJoinWidget.h"
#include "HG_GameInstance.h"
#include "../HJS/JS_SessionGameInstanceSubSystem.h"
#include "../KSW/UI/SW_CreateCreatorWidget.h"

void AHG_GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (SessionWidgetFactory)
	{
		SessionWidget = CreateWidget<UJS_SessionJoinWidget>(GetWorld(), SessionWidgetFactory);
		SessionWidget->AddToViewport();
		SessionWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (CreateCreatorWidgetFactory)
	{
		CreateCreatorWidget = CreateWidget<USW_CreateCreatorWidget>(GetWorld(), CreateCreatorWidgetFactory);
		CreateCreatorWidget->AddToViewport();
		CreateCreatorWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	UHG_GameInstance* gi = Cast<UHG_GameInstance>(GetWorld()->GetGameInstance());
	if (gi)
	{
		UJS_SessionGameInstanceSubSystem* si = gi->GetSubsystem<UJS_SessionGameInstanceSubSystem>();
		if (si)
		{
			si->FindOtherSessions();
		}
	}


}
