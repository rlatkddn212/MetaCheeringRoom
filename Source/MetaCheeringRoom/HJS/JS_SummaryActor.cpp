// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_SummaryActor.h"
#include "Components/BoxComponent.h"
#include "JS_SumarryRequestWidget.h"
#include "JS_SummaryResultWidget.h"
#include "../SHK/HG_Player.h"

// Sets default values
AJS_SummaryActor::AJS_SummaryActor()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(BoxComp);
}

// Called when the game starts or when spawned
void AJS_SummaryActor::BeginPlay()
{
	Super::BeginPlay();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AJS_SummaryActor::ComponentBeginOverlap);

	if (SummaryReqWidgetFactory)
	{
		SummaryReqWidget = CreateWidget<UJS_SumarryRequestWidget>(GetWorld(),SummaryReqWidgetFactory);
		if (SummaryReqWidget)
		{
			SummaryReqWidget->AddToViewport(30);
			SummaryReqWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (SummaryResultWidgetFactory)
	{
		SummaryResultWidget = CreateWidget<UJS_SummaryResultWidget>(GetWorld(), SummaryResultWidgetFactory);
		if (SummaryResultWidget)
		{
			SummaryResultWidget->AddToViewport(30);
		}
	}

}

void AJS_SummaryActor::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SummaryReqWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

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


