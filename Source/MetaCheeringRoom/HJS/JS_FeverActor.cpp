// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_FeverActor.h"
#include "JS_FeverWidget.h"
#include "../SHK/HG_Player.h"
#include "MetaCheeringRoom.h"
// Sets default values
AJS_FeverActor::AJS_FeverActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

}

// Called when the game starts or when spawned
void AJS_FeverActor::BeginPlay()
{
	Super::BeginPlay();
	
	auto* PC = GetWorld()->GetFirstPlayerController();
	if (PC && PC->HasAuthority())
	{
		SetOwner(PC->GetCharacter());
	}
	if (FeverWidgetFactory)
	{
		FeverWidget = CreateWidget<UJS_FeverWidget>(GetWorld(), FeverWidgetFactory);
		if (FeverWidget)
		{
			FeverWidget->AddToViewport(-1);
			FeverWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

// Called every frame
void AJS_FeverActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJS_FeverActor::FeverTimeStart()
{
	MulticastFeverTimeStart();
}

void AJS_FeverActor::MulticastFeverTimeStart_Implementation()
{
	PRINTLOG(TEXT("0000"));

	FeverWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	FeverWidget->bFeverStart = true;
	FeverWidget->CurrentTime = 0.f;
	FeverWidget->SetPercent(0.1f);
}