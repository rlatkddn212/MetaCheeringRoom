// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorObject/SW_CreatorRectLight.h"
#include "Components/RectLightComponent.h"

ASW_CreatorRectLight::ASW_CreatorRectLight()
{
	PrimaryActorTick.bCanEverTick = true;

	LightComp = CreateDefaultSubobject<URectLightComponent>(TEXT("LightComp"));
	LightComp->SetupAttachment(Root);

	LightColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void ASW_CreatorRectLight::OnChangeColor(FLinearColor Color)
{
	Super::OnChangeColor(Color);

	LightColor = Color;
	LightComp->SetLightColor(Color);
}

FLinearColor ASW_CreatorRectLight::GetColor()
{
	return LightComp->GetLightColor();
}

void ASW_CreatorRectLight::RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const
{
	Super::RecordJsonAdditionalInfo(RecordJsonObject);
	RecordJsonObject->SetNumberField(TEXT("ColorR"), LightColor.R);
	RecordJsonObject->SetNumberField(TEXT("ColorG"), LightColor.G);
	RecordJsonObject->SetNumberField(TEXT("ColorB"), LightColor.B);
	RecordJsonObject->SetNumberField(TEXT("ColorA"), LightColor.A);
}

void ASW_CreatorRectLight::SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject)
{
	Super::SetupJsonAdditionalInfo(SetupJsonObject);
	// 추가 정보를 설정한다.
	LightColor.R = SetupJsonObject->GetNumberField(TEXT("ColorR"));
	LightColor.G = SetupJsonObject->GetNumberField(TEXT("ColorG"));
	LightColor.B = SetupJsonObject->GetNumberField(TEXT("ColorB"));
	LightColor.A = SetupJsonObject->GetNumberField(TEXT("ColorA"));

	LightComp->SetLightColor(LightColor);
}
