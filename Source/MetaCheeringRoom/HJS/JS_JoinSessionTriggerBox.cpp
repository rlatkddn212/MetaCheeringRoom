// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_JoinSessionTriggerBox.h"
#include "../SHK/HG_GameModeBase.h"
#include "JS_SessionJoinWidget.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "../SHK/HG_Player.h"
#include "../SHK/HG_GameInstance.h"
#include "JS_SessionGameInstanceSubSystem.h"

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
		UI->PlayShowJoinSessionAnimation();
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

