// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorObject/SW_CreatorDirectionalLight.h"
#include "Engine/DirectionalLight.h"
#include "EngineUtils.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/LightComponent.h"

ASW_CreatorDirectionalLight::ASW_CreatorDirectionalLight()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASW_CreatorDirectionalLight::BeginPlay()
{
	Super::BeginPlay();


	// ��� ���忡 �ִ� ������Ʈ�� �˻��Ͽ� ��ī�̶���Ʈ�� �����´�.
	for (TActorIterator<ADirectionalLight> It(GetWorld()); It; ++It)
	{
		ADirectionalLight* directionalLight = *It;
		if (directionalLight)
		{
			LightComp = Cast<UDirectionalLightComponent>(directionalLight->GetLightComponent());
			break;
		}
	}
}
