// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/SW_CreatorSkeletonObject.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"

ASW_CreatorSkeletonObject::ASW_CreatorSkeletonObject()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void ASW_CreatorSkeletonObject::BeginPlay()
{
	Super::BeginPlay();

}

void ASW_CreatorSkeletonObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
