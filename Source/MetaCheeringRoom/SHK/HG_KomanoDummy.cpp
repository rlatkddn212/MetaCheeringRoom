// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_KomanoDummy.h"
#include "Net/UnrealNetwork.h"
#include "Components/ChildActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AHG_KomanoDummy::AHG_KomanoDummy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh->SetGenerateOverlapEvents(true);
	
	bReplicates = true;
	SetReplicateMovement(true);
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AHG_KomanoDummy::BeginPlay()
{
	Super::BeginPlay();

}

void AHG_KomanoDummy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

// 	DOREPLIFETIME(AHG_KomanoDummy, bShake);
// 	DOREPLIFETIME(AHG_KomanoDummy, bSit);
// 	DOREPLIFETIME(AHG_KomanoDummy, bIdle);
}

void AHG_KomanoDummy::Multicast_SetStateSit_Implementation(bool Value)
{
	bSit = Value;
}

void AHG_KomanoDummy::Multicast_SetStateIdle_Implementation(bool Value)
{
	bIdle = Value;
}

void AHG_KomanoDummy::Multicast_SetStateShake_Implementation(bool Value)
{
	bShake = Value;
}

void AHG_KomanoDummy::ReturnOriginCondition()
{
	ServerRPC_SetStateSit(true);
	ServerRPC_SetStateIdle(false);
	ServerRPC_SetStateShake(false);
	ChangeCSOrigin();
}

void AHG_KomanoDummy::CheerSurfing()
{
	ServerRPC_SetStateSit(false);
	ServerRPC_SetStateIdle(true);
	ServerRPC_SetStateShake(true);

	ChangeCheeringStickColor();

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, this, &AHG_KomanoDummy::ReturnOriginCondition, 0.5f, false);
}



void AHG_KomanoDummy::ServerRPC_SetStateSit(bool Value)
{
	bSit = Value;
	//Multicast_SetStateSit(Value);
}

void AHG_KomanoDummy::ServerRPC_SetStateIdle(bool Value)
{
	bIdle = Value;
	//Multicast_SetStateIdle(Value);
}

void AHG_KomanoDummy::ServerRPC_SetStateShake(bool Value)
{
	bShake = Value;
	//Multicast_SetStateShake(Value);
}

