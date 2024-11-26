// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_TeemoDummy.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "HG_Player.h"

// Sets default values
AHG_TeemoDummy::AHG_TeemoDummy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	// 
	// 	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	// 	MeshComp->SetupAttachment(RootComponent);

	BoxComp->SetGenerateOverlapEvents(true);
	BoxComp->SetCollisionProfileName(TEXT("TeemoDummy"));

}

// Called when the game starts or when spawned
void AHG_TeemoDummy::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AHG_TeemoDummy::OnMyBeginOverlap);

	Direction = GetActorForwardVector();
}

// Called every frame
void AHG_TeemoDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + DeltaTime * Direction.GetSafeNormal() * Speed);

	FHitResult OutHit;
	FVector Start = GetActorLocation() - 20.0f;
	FVector End = Start + GetActorUpVector() * -1 * 150.0f;

	ECollisionChannel TC = ECC_WorldDynamic;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, TC, Params);

	if (bHit)
	{
		float Z = OutHit.ImpactPoint.Z;
		FVector CurDir = GetActorLocation();
		CurDir.Z = Z + 40.0f;
		SetActorLocation(CurDir);
	}
	if (GetActorLocation().Z > 100.0f)
	{
		FVector CurDir = GetActorLocation();
		CurDir.Z = 48.5f;
		SetActorLocation(CurDir);
	}
}

void AHG_TeemoDummy::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Direction = -1 * Direction;
	Direction.X += FMath::FRandRange(-0.5, 0.5);
	Direction.Y += FMath::FRandRange(-0.5, 0.5);
	SetActorRotation(Direction.GetSafeNormal().Rotation());
}

