// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorObject/SW_CreatorSpotLight.h"
#include "Components/SpotLightComponent.h"

ASW_CreatorSpotLight::ASW_CreatorSpotLight()
{
	PrimaryActorTick.bCanEverTick = true;
	
	LightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("LightComp"));
	LightComp->SetupAttachment(Root);

	LightColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void ASW_CreatorSpotLight::OnChangeProperty(int32 id, UCreatorPropertyBase* CreatorProperty)
{
	Super::OnChangeProperty(id, CreatorProperty);

	if (id == 1)
	{
		UCreatorColorProperty* ColorProperty = Cast<UCreatorColorProperty>(CreatorProperty);
		LightColor = ColorProperty->Value;
		LightComp->SetLightColor(LightColor);
	}
}

UCreatorPropertyBase* ASW_CreatorSpotLight::GetProperty(int32 id)
{
	if (PropertyMap.Contains(id))
	{
		return PropertyMap[id];
	}
	return nullptr;
}

void ASW_CreatorSpotLight::RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const
{
	Super::RecordJsonAdditionalInfo(RecordJsonObject);
	RecordJsonObject->SetNumberField(TEXT("ColorR"), LightColor.R);
	RecordJsonObject->SetNumberField(TEXT("ColorG"), LightColor.G);
	RecordJsonObject->SetNumberField(TEXT("ColorB"), LightColor.B);
	RecordJsonObject->SetNumberField(TEXT("ColorA"), LightColor.A);
}

void ASW_CreatorSpotLight::SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject)
{
	Super::SetupJsonAdditionalInfo(SetupJsonObject);
	// 추가 정보를 설정한다.
	LightColor.R = SetupJsonObject->GetNumberField(TEXT("ColorR"));
	LightColor.G = SetupJsonObject->GetNumberField(TEXT("ColorG"));
	LightColor.B = SetupJsonObject->GetNumberField(TEXT("ColorB"));
	LightColor.A = SetupJsonObject->GetNumberField(TEXT("ColorA"));

	UCreatorColorProperty* Property = NewObject<UCreatorColorProperty>(); Property->Value = LightColor; PropertyMap.Add(1, Property);

	LightComp->SetLightColor(LightColor);
}
