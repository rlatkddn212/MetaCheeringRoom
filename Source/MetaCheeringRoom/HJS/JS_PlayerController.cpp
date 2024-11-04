// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "JS_TotoActor.h"
#include "JS_Screen.h"
#include "JS_NetComponent.h"
#include "JS_ExitWidget.h"

AJS_PlayerController::AJS_PlayerController()
{
}

void AJS_PlayerController::BeginPlay()
{
	if(HasAuthority())
	{
		TotoActor = Cast<AJS_TotoActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_TotoActor::StaticClass()));
		ScreenActor = Cast<AJS_Screen>(UGameplayStatics::GetActorOfClass(GetWorld(),AJS_Screen::StaticClass()));
	}
	else
	{
		MyScreenActor = Cast<AJS_Screen>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_Screen::StaticClass()));
	}

	if (ExitWidgetFactory)
	{
		ExitWidget = CreateWidget<UJS_ExitWidget>(GetWorld(), ExitWidgetFactory);
		if (ExitWidget)
		{
			ExitWidget->AddToViewport(15);
			ExitWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	StartTalking();
}

void AJS_PlayerController::ServerHandleBettingToTo_Implementation(int32 point, int32 select, const FString& UserId)
{
	if (TotoActor)
	{
		TotoActor->ServerBettingToto(point, select, UserId);
	}
	else
	{
		TotoActor = Cast<AJS_TotoActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_TotoActor::StaticClass()));
		if (TotoActor)
		{
			TotoActor->ServerBettingToto(point, select, UserId);
		}
	}
}


void AJS_PlayerController::ServerHandleVideoPlay_Implementation()
{
	if (ScreenActor)
	{
		FString URL = ScreenActor->NetComp->VideoURL;
		FString StreamID = ScreenActor->NetComp->StreamID;
		int32 SegmentNumber = ScreenActor->NetComp->SegmentNumber - 1;
		ClientHandleVideoPlay(URL, StreamID, SegmentNumber);
	}
}

void AJS_PlayerController::ClientHandleVideoPlay_Implementation(const FString& URL, const FString& streamID, int32 segmentNumber)
{
	if (MyScreenActor)
	{
		MyScreenActor->NetComp->RepVideoURLWrite(URL,streamID,segmentNumber);
		MyScreenActor->PlayMedia(streamID);
	}
}
