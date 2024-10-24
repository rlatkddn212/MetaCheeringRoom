// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_ToToWidgetTriggerBox.h"
#include "Kismet/GameplayStatics.h"
#include "JS_TotoActor.h"
#include "JS_ToToWidget.h"
#include "Components/BoxComponent.h"
#include "../MetaCheeringRoom.h"

// Sets default values
AJS_ToToWidgetTriggerBox::AJS_ToToWidgetTriggerBox()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(BoxComp);
}

// Called when the game starts or when spawned
void AJS_ToToWidgetTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	
	ToToActor = Cast<AJS_TotoActor>(UGameplayStatics::GetActorOfClass(GetWorld(),AJS_TotoActor::StaticClass()));
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AJS_ToToWidgetTriggerBox::ComponentBeginOverlap);
}

void AJS_ToToWidgetTriggerBox::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ToToActor)
	{
		UJS_ToToWidget* ToToWidget = ToToActor->ToToWidget;
		ToToWidget->ToToInitSetting();
	}
}

