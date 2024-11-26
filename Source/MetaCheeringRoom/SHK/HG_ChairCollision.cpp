// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_ChairCollision.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "HG_Player.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AHG_ChairCollision::AHG_ChairCollision()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);


}
// Called when the game starts or when spawned
void AHG_ChairCollision::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&AHG_ChairCollision::OnMyBeginOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this,&AHG_ChairCollision::OnMyEndOverlap);
}

void AHG_ChairCollision::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHG_ChairCollision,IsSeatTaken);
}

// Called every frame
void AHG_ChairCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OverlapedPlayer)
	{
		if((OverlapedPlayer->GetActorLocation() - this->GetActorLocation()).Length() > 300.0f)
		{
			if(OverlapedPlayer->DetectChair == this) OverlapedPlayer->DetectChair = nullptr;
			OverlapedPlayer = nullptr;
		}
	}
}

void AHG_ChairCollision::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto* Player = Cast<AHG_Player>(OtherActor))
	{
		Player->DetectChair = this;
	}
}

void AHG_ChairCollision::OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (auto* Player = Cast<AHG_Player>(OtherActor))
	{
		OverlapedPlayer = Player;
	}
}

