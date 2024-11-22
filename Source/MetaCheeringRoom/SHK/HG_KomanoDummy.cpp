// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_KomanoDummy.h"
#include "Net/UnrealNetwork.h"
#include "Components/ChildActorComponent.h"

// Sets default values
AHG_KomanoDummy::AHG_KomanoDummy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHG_KomanoDummy::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHG_KomanoDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//
//// Called to bind functionality to input
//void AHG_KomanoDummy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

void AHG_KomanoDummy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHG_KomanoDummy, bShake);
	DOREPLIFETIME(AHG_KomanoDummy, bSit);
	DOREPLIFETIME(AHG_KomanoDummy, bIdle);
}



void AHG_KomanoDummy::ServerRPC_SetStateSit_Implementation(bool Value)
{
	bSit = Value;
}

void AHG_KomanoDummy::ServerRPC_SetStateIdle_Implementation(bool Value)
{
	bIdle = Value;
}

void AHG_KomanoDummy::ServerRPC_SetStateShake_Implementation(bool Value)
{
	bShake = Value;
}

