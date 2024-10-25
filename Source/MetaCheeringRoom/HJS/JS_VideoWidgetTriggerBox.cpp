// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_VideoWidgetTriggerBox.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "JS_Screen.h"
#include "GameFramework/Character.h"
#include "VideoWidget.h"
// Sets default values
AJS_VideoWidgetTriggerBox::AJS_VideoWidgetTriggerBox()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(BoxComp);
}

// Called when the game starts or when spawned
void AJS_VideoWidgetTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	ScreenActor = Cast<AJS_Screen>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_Screen::StaticClass()));
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AJS_VideoWidgetTriggerBox::ComponentBeginOverlap);
}

void AJS_VideoWidgetTriggerBox::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ScreenActor)
	{
		ACharacter* Character = Cast<ACharacter>(OtherActor);
		if (Character && Character->IsLocallyControlled() && Character->HasAuthority())
		{
			UVideoWidget* VideoWidget = ScreenActor->VideoWidget;
			if (VideoWidget)
			{
				VideoWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				APlayerController* PC = GetWorld()->GetFirstPlayerController();
				PC->SetShowMouseCursor(true);
				PC->SetInputMode(FInputModeUIOnly());
			}
		}
	}
}