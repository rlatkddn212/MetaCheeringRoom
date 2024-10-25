// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_JoinSessionTriggerBox.h"
#include "../SHK/HG_GameModeBase.h"
#include "JS_SessionJoinWidget.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

// Sets default values
AJS_JoinSessionTriggerBox::AJS_JoinSessionTriggerBox()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(BoxComp);
}

// Called when the game starts or when spawned
void AJS_JoinSessionTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AJS_JoinSessionTriggerBox::ComponentBeginOverlap);
}

void AJS_JoinSessionTriggerBox::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (!Character)
	{
		return;
	}

	AHG_GameModeBase* GM = Cast<AHG_GameModeBase>(GetWorld()->GetAuthGameMode());

	if (GM)
	{
		UJS_SessionJoinWidget* UI = GM->SessionWidget;

		UI->SetVisibility(ESlateVisibility::Visible);
		UI->MenuSwitching(1);

		APlayerController* PC = GetWorld()->GetFirstPlayerController();

		if (PC)
		{
			PC->SetShowMouseCursor(true);
			PC->SetInputMode(FInputModeUIOnly());
		}
	}
}

