// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorObject/SW_CreatorDirectionalLight.h"
#include "Engine/DirectionalLight.h"
#include "EngineUtils.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/LightComponent.h"
#include "UObject/UObjectGlobals.h"

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

void ASW_CreatorDirectionalLight::OnChangeProperty(int32 id, UCreatorPropertyBase* CreatorProperty)
{
	Super::OnChangeProperty(id, CreatorProperty);

	if (id == 1)
	{
		//UCreatorColorProperty* ColorProperty = Cast<UCreatorColorProperty>(CreatorProperty);
		UCreatorColorProperty* ColorProperty = Cast<UCreatorColorProperty>(CreatorProperty);
		LightColor = ColorProperty->Value;
		LightComp->SetLightColor(LightColor);
	}

	if (id == 2)
	{
		UCreatorFloatProperty* IntensityProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		LightComp->SetIntensity(IntensityProperty->Value);
	}

	if (id == 3)
	{
		UCreatorBoolProperty* CastShadowProperty = Cast<UCreatorBoolProperty>(CreatorProperty);
		LightComp->SetCastShadows(CastShadowProperty->Value);
	}
}

TMap<int32, UCreatorPropertyBase*> ASW_CreatorDirectionalLight::GetPropertyMap()
{
	UCreatorColorProperty* Property = NewObject<UCreatorColorProperty>();
	Property->PropertyName = TEXT("Color");
	Property->Value = LightColor;
	AddProperty(1, Property);

	UCreatorFloatProperty* Property2 = NewObject<UCreatorFloatProperty>();
	Property2->PropertyName = TEXT("Intensity");
	Property2->Value = LightComp->Intensity;
	AddProperty(2, Property2);

	UCreatorBoolProperty* Property3 = NewObject<UCreatorBoolProperty>();
	Property3->PropertyName = TEXT("CastShadow");
	Property3->Value = LightComp->CastShadows;
	AddProperty(3, Property3);

	return PropertyMap;
}

void ASW_CreatorDirectionalLight::RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const
{
	Super::RecordJsonAdditionalInfo(RecordJsonObject);
	RecordJsonObject->SetNumberField(TEXT("ColorR"), LightColor.R);
	RecordJsonObject->SetNumberField(TEXT("ColorG"), LightColor.G);
	RecordJsonObject->SetNumberField(TEXT("ColorB"), LightColor.B);
	RecordJsonObject->SetNumberField(TEXT("ColorA"), LightColor.A);

	RecordJsonObject->SetNumberField(TEXT("Intensity"), LightComp->Intensity);
	RecordJsonObject->SetBoolField(TEXT("CastShadow"), LightComp->CastShadows);
}

void ASW_CreatorDirectionalLight::SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject)
{
	Super::SetupJsonAdditionalInfo(SetupJsonObject);
	// 추가 정보를 설정한다.
	LightColor.R = SetupJsonObject->GetNumberField(TEXT("ColorR"));
	LightColor.G = SetupJsonObject->GetNumberField(TEXT("ColorG"));
	LightColor.B = SetupJsonObject->GetNumberField(TEXT("ColorB"));
	LightColor.A = SetupJsonObject->GetNumberField(TEXT("ColorA"));

	LightComp->SetLightColor(LightColor);

	LightIntensity = SetupJsonObject->GetNumberField(TEXT("Intensity"));
	LightComp->SetIntensity(LightIntensity);

	CastShadow = SetupJsonObject->GetBoolField(TEXT("CastShadow"));
	LightComp->SetCastShadows(CastShadow);
}