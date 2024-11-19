// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/Creator/SW_CreatorGameState.h"
#include "../SW_CreatorPlayerController.h"
#include "../CreatorMapSubsystem.h"
#include "../CreatorObject/SW_CreatorObject.h"
#include "../CreatorStorageSubsystem.h"

void ASW_CreatorGameState::Multicast_AddCreatingDummyObject_Implementation(class ASW_CreatorObject* NewCreatingObject)
{
	UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
	NewCreatingObject->SetFileName(NewCreatingObject->CreatingObjectData->ItemName);
	system->AddObject(NewCreatingObject);

	ASW_CreatorPlayerController* PC = Cast<ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->ReloadHierarchy();
	}
}

void ASW_CreatorGameState::Multicast_DeleteObject_Implementation(class ASW_CreatorObject* DeleteObject)
{
	ASW_CreatorPlayerController* PC = Cast<ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->UnSelectServerDeleteObject(DeleteObject);
	}

	UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
	system->RemoveObject(DeleteObject, true);

	if (PC)
	{
		PC->ReloadHierarchy();
	}
}
