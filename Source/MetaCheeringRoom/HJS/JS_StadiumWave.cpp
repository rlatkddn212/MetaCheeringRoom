// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_StadiumWave.h"
#include "Components/BoxComponent.h"
#include "MetaCheeringRoom.h"
// Sets default values
AJS_StadiumWave::AJS_StadiumWave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(MeshComp);

}

// Called when the game starts or when spawned
void AJS_StadiumWave::BeginPlay()
{
	Super::BeginPlay();
    BoxComp->OnComponentBeginOverlap.AddDynamic(this,&AJS_StadiumWave::ComponentBeginOverlap);
    BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    if (HasAuthority())
    {
        SetOwner(GetWorld()->GetFirstPlayerController()->GetPawn());
        TurnStart();
    }
}

// Called every frame
void AJS_StadiumWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    OnTurn(DeltaTime);
}

void AJS_StadiumWave::TurnStart()
{
    MulticastTurnStart();
}

void AJS_StadiumWave::MulticastTurnStart_Implementation()
{
    bTurn = true;
    CurrentRotation = 0.0f;
    BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AJS_StadiumWave::OnTurn(float DeltaTime)
{
    if (!bTurn) return;

    // 현재 회전 각도 업데이트
    CurrentRotation += RotationSpeed * DeltaTime;

    // 새로운 회전값 설정
    FRotator NewRotation = GetActorRotation();
    NewRotation.Yaw = CurrentRotation;
    SetActorRotation(NewRotation);

    // 360도 회전 완료 체크
    if (CurrentRotation >= 360.0f)
    {
        bTurn = false;
        CurrentRotation = 0.0f;
        BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

void AJS_StadiumWave::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    i++;
    PRINTLOG(TEXT("BeginOverlap"));
    UE_LOG(LogTemp,Warning,TEXT("ONERONMER"));
}

