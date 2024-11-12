// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorObject/SW_CreatorEffect.h"
#include "Particles/ParticleSystemComponent.h"

ASW_CreatorEffect::ASW_CreatorEffect()
{
	PrimaryActorTick.bCanEverTick = true;

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComp->SetupAttachment(Root);
}
