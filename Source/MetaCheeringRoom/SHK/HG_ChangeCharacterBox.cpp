// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_ChangeCharacterBox.h"
#include "Components/BoxComponent.h"
#include "HG_Player.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AHG_ChangeCharacterBox::AHG_ChangeCharacterBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	BoxComp->SetGenerateOverlapEvents(true);
	BoxComp->SetCollisionProfileName(TEXT("CharaterChange"));
}

// Called when the game starts or when spawned
void AHG_ChangeCharacterBox::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this,&AHG_ChangeCharacterBox::OnMyBeginOverlap);
}

// Called every frame
void AHG_ChangeCharacterBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHG_ChangeCharacterBox::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AHG_Player>())
	{
		UE_LOG(LogTemp,Warning,TEXT("111"));
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(),WidgetFactory);
		
		if (Widget)
		{
			Widget->AddToViewport();
		}
	}
}

