// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorObject/SW_CreatorRectLight.h"
#include "Components/RectLightComponent.h"

ASW_CreatorRectLight::ASW_CreatorRectLight()
{
	PrimaryActorTick.bCanEverTick = true;

	LightComp = CreateDefaultSubobject<URectLightComponent>(TEXT("LightComp"));
	LightComp->SetupAttachment(Root);
}
