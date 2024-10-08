// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_Screen.h"
#include "JS_NetComponent.h"

// Sets default values
AJS_Screen::AJS_Screen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NetComp = CreateDefaultSubobject<UJS_NetComponent>(TEXT("NetComp"));

}

// Called when the game starts or when spawned
void AJS_Screen::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJS_Screen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

