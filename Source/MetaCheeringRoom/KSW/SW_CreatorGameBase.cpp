// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/SW_CreatorGameBase.h"
#include "CreatorMapSubsystem.h"
#include "CreatorStorageSubsystem.h"

ASW_CreatorGameBase::ASW_CreatorGameBase()
{

}

void ASW_CreatorGameBase::BeginPlay()
{
	Super::BeginPlay();

	UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
	system->LoadMap();
}
