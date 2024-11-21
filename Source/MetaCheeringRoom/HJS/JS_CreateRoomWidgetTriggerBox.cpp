// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_CreateRoomWidgetTriggerBox.h"
#include "Components/BoxComponent.h"
#include "../SHK/HG_GameModeBase.h"
#include "JS_SessionJoinWidget.h"
#include "../SHK/HG_Player.h"
#include "JS_CreateRoomWidget.h"

// Sets default values
AJS_CreateRoomWidgetTriggerBox::AJS_CreateRoomWidgetTriggerBox()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(BoxComp);
}

// Called when the game starts or when spawned
void AJS_CreateRoomWidgetTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AJS_CreateRoomWidgetTriggerBox::ComponentBeginOverlap);
}

void AJS_CreateRoomWidgetTriggerBox::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (!Character)
	{
		return;
	}

	AHG_GameModeBase* GM = Cast<AHG_GameModeBase>(GetWorld()->GetAuthGameMode());

	if (GM)
	{
		UJS_CreateRoomWidget* UI = GM->CreateRoomWidget;

		UI->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UI->PlayShowWidgetAnimation();
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