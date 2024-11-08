// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorObject/SW_CreatorSpotLight.h"
#include "Components/SpotLightComponent.h"

ASW_CreatorSpotLight::ASW_CreatorSpotLight()
{
	PrimaryActorTick.bCanEverTick = true;
	
	LightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("LightComp"));
	LightComp->SetupAttachment(Root);
}
