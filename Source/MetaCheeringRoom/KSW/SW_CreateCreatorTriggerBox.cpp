// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/SW_CreateCreatorTriggerBox.h"
#include "../SHK/HG_GameModeBase.h"
#include "UI/SW_CreateCreatorWidget.h"
#include "Components/BoxComponent.h"
#include "../SHK/HG_Player.h"

// Sets default values
ASW_CreateCreatorTriggerBox::ASW_CreateCreatorTriggerBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(BoxComp);
}

// Called when the game starts or when spawned
void ASW_CreateCreatorTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ASW_CreateCreatorTriggerBox::ComponentBeginOverlap);
}

// Called every frame
void ASW_CreateCreatorTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASW_CreateCreatorTriggerBox::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AHG_GameModeBase* GM = Cast<AHG_GameModeBase>(GetWorld()->GetAuthGameMode());

	if (GM)
	{
		USW_CreateCreatorWidget* UI = GM->CreateCreatorWidget;
		UI->SetVisibility(ESlateVisibility::Visible);

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

