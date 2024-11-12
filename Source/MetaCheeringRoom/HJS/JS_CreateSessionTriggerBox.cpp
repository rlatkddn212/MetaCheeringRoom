// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_CreateSessionTriggerBox.h"
#include "Components/BoxComponent.h"
#include "../SHK/HG_GameModeBase.h"
#include "JS_SessionJoinWidget.h"
#include "MetaCheeringRoom.h"
#include "../SHK/HG_Player.h"
// Sets default values
AJS_CreateSessionTriggerBox::AJS_CreateSessionTriggerBox()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(BoxComp);
}

// Called when the game starts or when spawned
void AJS_CreateSessionTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AJS_CreateSessionTriggerBox::ComponentBeginOverlap);
}

void AJS_CreateSessionTriggerBox::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AHG_GameModeBase* GM =Cast<AHG_GameModeBase>(GetWorld()->GetAuthGameMode());

	if (GM)
	{
		UJS_SessionJoinWidget* UI = GM->SessionWidget;
		UI->SetVisibility(ESlateVisibility::Visible);
		UI->MenuSwitching(0);
		UI->SetupMapData();
		UI->PlayShowMakeSessionAnimation();
		APlayerController* PC = GetWorld()->GetFirstPlayerController();

		if (PC)
		{
			PC->SetShowMouseCursor(true);
			PC->SetInputMode(FInputModeUIOnly());
			AHG_Player* Player = Cast<AHG_Player>(PC->GetCharacter());
			if (Player)
			{
				Player->Direction = FVector::ZeroVector;
				Player->bCanMove = false;
			}
		}
	}
}

