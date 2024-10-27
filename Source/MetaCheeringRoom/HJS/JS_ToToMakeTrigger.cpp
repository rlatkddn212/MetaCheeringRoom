// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_ToToMakeTrigger.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "JS_TotoActor.h"
#include "ToToMakeWidget.h"
#include "GameFramework/Character.h"
#include "../SHK/HG_Player.h"

// Sets default values
AJS_ToToMakeTrigger::AJS_ToToMakeTrigger()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(BoxComp);
}

// Called when the game starts or when spawned
void AJS_ToToMakeTrigger::BeginPlay()
{
	Super::BeginPlay();
	ToToActor = Cast<AJS_TotoActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_TotoActor::StaticClass()));
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AJS_ToToMakeTrigger::ComponentBeginOverlap);
}

void AJS_ToToMakeTrigger::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ToToActor)
	{
		ACharacter* Character = Cast<ACharacter>(OtherActor);
		if (Character && Character->IsLocallyControlled() &&  Character->HasAuthority())
		{
			UToToMakeWidget* TotoMakeWidget = ToToActor->TotoMakeWidget;
			if (TotoMakeWidget && TotoMakeWidget->bOpen)
			{
				TotoMakeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
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
	}
}


