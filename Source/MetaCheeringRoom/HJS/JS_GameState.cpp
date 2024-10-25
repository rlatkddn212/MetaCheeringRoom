// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_GameState.h"
#include "Kismet/GameplayStatics.h"
#include "JS_TotoActor.h"

AJS_GameState::AJS_GameState()
{
	bReplicates = true;
}

void AJS_GameState::BeginPlay()
{
	TotoActor = Cast<AJS_TotoActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_TotoActor::StaticClass()));
}

void AJS_GameState::HandleBettingToTo(int32 point, int32 select, const FString& UserId)
{
	ServerHandleBettingToTo(point,select,UserId);
}

void AJS_GameState::ServerHandleBettingToTo_Implementation(int32 point, int32 select, const FString& UserId)
{
	if (TotoActor)
	{
		TotoActor->ServerBettingToto(point,select, UserId);
	}
}
