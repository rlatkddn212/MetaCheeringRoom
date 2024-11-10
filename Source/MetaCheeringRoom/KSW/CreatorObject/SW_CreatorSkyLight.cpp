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

	// 모든 월드에 있는 오브젝트를 검사하여 스카이라이트를 가져온다.
	for (TActorIterator<ASkyLight> It(GetWorld()); It; ++It)
	{
		ASkyLight* SkyLightActor = *It;
		if (SkyLightActor)
		{
			LightComp = SkyLightActor->GetLightComponent();
			break;
		}
	}

	if (LightComp)
	{
		LightColor = LightComp->GetLightColor();
	}
}

void ASW_CreatorSkyLight::OnChangeColor(FLinearColor Color)
{
	LightColor = Color;
	LightComp->SetLightColor(Color);
}

FLinearColor ASW_CreatorSkyLight::GetColor()
{
	return LightColor;
}

void ASW_CreatorSkyLight::RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const
{
	Super::RecordJsonAdditionalInfo(RecordJsonObject);
	RecordJsonObject->SetNumberField(TEXT("ColorR"), LightColor.R);
	RecordJsonObject->SetNumberField(TEXT("ColorG"), LightColor.G);
	RecordJsonObject->SetNumberField(TEXT("ColorB"), LightColor.B);
	RecordJsonObject->SetNumberField(TEXT("ColorA"), LightColor.A);
}

void ASW_CreatorSkyLight::SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject)
{
	Super::SetupJsonAdditionalInfo(SetupJsonObject);
	// 추가 정보를 설정한다.
	LightColor.R = SetupJsonObject->GetNumberField(TEXT("ColorR"));
	LightColor.G = SetupJsonObject->GetNumberField(TEXT("ColorG"));
	LightColor.B = SetupJsonObject->GetNumberField(TEXT("ColorB"));
	LightColor.A = SetupJsonObject->GetNumberField(TEXT("ColorA"));

	OnChangeColor(LightColor);
}
