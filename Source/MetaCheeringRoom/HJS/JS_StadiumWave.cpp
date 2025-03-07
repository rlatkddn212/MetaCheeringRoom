// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_StadiumWave.h"
#include "Components/BoxComponent.h"
#include "MetaCheeringRoom.h"
#include "../SHK/HG_KomanoDummy.h"
#include "Kismet/GameplayStatics.h"
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
    CurrentRotation = 270.f;
    BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    UGameplayStatics::PlaySound2D(GetWorld(), WaveSound);
}

void AJS_StadiumWave::OnTurn(float DeltaTime)
{
    if (!bTurn) return;

    // 현재 회전 각도 업데이트s
    CurrentRotation += RotationSpeed * DeltaTime;

    // 새로운 회전값 설정
    FRotator NewRotation = GetActorRotation();
    NewRotation.Yaw = CurrentRotation;
    SetActorRotation(NewRotation);

    // 360도 회전 완료 체크
    if (CurrentRotation >= 630.0f)
    {
        bTurn = false;
        CurrentRotation = 270.f;
        BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

void AJS_StadiumWave::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AHG_KomanoDummy* Dummy = Cast<AHG_KomanoDummy>(OtherActor);
    if (Dummy)
    {   
        Dummy->CheerSurfing();
    }
}