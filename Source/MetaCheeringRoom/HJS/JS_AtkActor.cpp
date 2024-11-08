// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_AtkActor.h"

// Sets default values
AJS_AtkActor::AJS_AtkActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SkelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkelMesh"));
}

// Called when the game starts or when spawned
void AJS_AtkActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJS_AtkActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

