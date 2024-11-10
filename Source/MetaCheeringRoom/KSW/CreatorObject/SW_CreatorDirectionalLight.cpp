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

	// 모든 월드에 있는 오브젝트를 검사하여 스카이라이트를 가져온다.
	for (TActorIterator<ADirectionalLight> It(GetWorld()); It; ++It)
	{
		ADirectionalLight* directionalLight = *It;
		if (directionalLight)
		{
			LightComp = Cast<UDirectionalLightComponent>(directionalLight->GetLightComponent());
			break;
		}
	}

	LightColor = LightComp->GetLightColor();
}

void ASW_CreatorDirectionalLight::OnChangeColor(FLinearColor Color)
{
	LightColor = Color;
	LightComp->SetLightColor(Color);
}

FLinearColor ASW_CreatorDirectionalLight::GetColor()
{
	return LightColor;
}

void ASW_CreatorDirectionalLight::RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const
{
	Super::RecordJsonAdditionalInfo(RecordJsonObject);
	RecordJsonObject->SetNumberField(TEXT("ColorR"), LightColor.R);
	RecordJsonObject->SetNumberField(TEXT("ColorG"), LightColor.G);
	RecordJsonObject->SetNumberField(TEXT("ColorB"), LightColor.B);
	RecordJsonObject->SetNumberField(TEXT("ColorA"), LightColor.A);
}

void ASW_CreatorDirectionalLight::SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject)
{
	Super::SetupJsonAdditionalInfo(SetupJsonObject);
	// 추가 정보를 설정한다.
	LightColor.R = SetupJsonObject->GetNumberField(TEXT("ColorR"));
	LightColor.G = SetupJsonObject->GetNumberField(TEXT("ColorG"));
	LightColor.B = SetupJsonObject->GetNumberField(TEXT("ColorB"));
	LightColor.A = SetupJsonObject->GetNumberField(TEXT("ColorA"));

	OnChangeColor(LightColor);
}