// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_StoreTriggerBox.h"
#include "HG_StoreWidget.h"
#include "Components/BoxComponent.h"
#include "HG_Player.h"

// Sets default values
AHG_StoreTriggerBox::AHG_StoreTriggerBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionComp"));
	SetRootComponent(BoxCollisionComp);
	BoxCollisionComp->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
}

// Called when the game starts or when spawned
void AHG_StoreTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	BoxCollisionComp->OnComponentBeginOverlap.AddDynamic(this,&AHG_StoreTriggerBox::OnMyBeginOverlap);

}

// Called every frame
void AHG_StoreTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHG_StoreTriggerBox::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto* Player = Cast<AHG_Player>(OtherActor))
	{
		Player->EnterTheStore();
	}
}
