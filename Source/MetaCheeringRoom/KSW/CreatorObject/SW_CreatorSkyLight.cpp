// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorObject/SW_CreatorSkyLight.h"
#include "Components/SkyLightComponent.h"
#include "Engine/SkyLight.h"
#include "EngineUtils.h"

ASW_CreatorSkyLight::ASW_CreatorSkyLight()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASW_CreatorSkyLight::BeginPlay()
{
	Super::BeginPlay();

	// ��� ���忡 �ִ� ������Ʈ�� �˻��Ͽ� ��ī�̶���Ʈ�� �����´�.
	for (TActorIterator<ASkyLight> It(GetWorld()); It; ++It)
	{
		ASkyLight* SkyLightActor = *It;
		if (SkyLightActor)
		{
			LightComp = SkyLightActor->GetLightComponent();
			break;
		}
	}
}
