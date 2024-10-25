// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "JS_TotoActor.h"

AJS_PlayerController::AJS_PlayerController()
{
}

void AJS_PlayerController::BeginPlay()
{
	if(HasAuthority())
	{
		TotoActor = Cast<AJS_TotoActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_TotoActor::StaticClass()));
	}
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
