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

	if (id == 2)
	{
		UCreatorFloatProperty* IntensityProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		LightComp->SetIntensity(IntensityProperty->Value);
	}

	if (id == 3)
	{
		UCreatorFloatProperty* AttenuationRadiusProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		LightComp->SetAttenuationRadius(AttenuationRadiusProperty->Value);
	
	}

	if (id == 4)
	{
		UCreatorFloatProperty* OuterConeAngleProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		LightComp->SetOuterConeAngle(OuterConeAngleProperty->Value);
	}

	if (id == 5)
	{
		UCreatorFloatProperty* InnerConeAngleProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		LightComp->SetInnerConeAngle(InnerConeAngleProperty->Value);
	}
}

TMap<int32, UCreatorPropertyBase*> ASW_CreatorSpotLight::GetPropertyMap()
{
	UCreatorColorProperty* Property = NewObject<UCreatorColorProperty>(); 
	Property->PropertyName = TEXT("Color");
	Property->Value = LightColor;
	AddProperty(1, Property);

	UCreatorFloatProperty* Property2 = NewObject<UCreatorFloatProperty>();
	Property2->PropertyName = TEXT("Intensity");
	Property2->Value = LightComp->Intensity;
	AddProperty(2, Property2);

	UCreatorFloatProperty* Property3 = NewObject<UCreatorFloatProperty>();
	Property3->PropertyName = TEXT("AttenuationRadius");
	Property3->Value = LightComp->AttenuationRadius;
	AddProperty(3, Property3);

	UCreatorFloatProperty* Property4 = NewObject<UCreatorFloatProperty>();
	Property4->PropertyName = TEXT("OuterConeAngle");
	Property4->Value = LightComp->OuterConeAngle;
	AddProperty(4, Property4);

	UCreatorFloatProperty* Property5 = NewObject<UCreatorFloatProperty>();
	Property5->PropertyName = TEXT("InnerConeAngle");
	Property5->Value = LightComp->InnerConeAngle;
	AddProperty(5, Property5);

	return PropertyMap;
}

void ASW_CreatorSpotLight::RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const
{
	Super::RecordJsonAdditionalInfo(RecordJsonObject);
	RecordJsonObject->SetNumberField(TEXT("ColorR"), LightColor.R);
	RecordJsonObject->SetNumberField(TEXT("ColorG"), LightColor.G);
	RecordJsonObject->SetNumberField(TEXT("ColorB"), LightColor.B);
	RecordJsonObject->SetNumberField(TEXT("ColorA"), LightColor.A);

	RecordJsonObject->SetNumberField(TEXT("Intensity"), LightComp->Intensity);
	RecordJsonObject->SetNumberField(TEXT("AttenuationRadius"), LightComp->AttenuationRadius);
	RecordJsonObject->SetNumberField(TEXT("OuterConeAngle"), LightComp->OuterConeAngle);
	RecordJsonObject->SetNumberField(TEXT("InnerConeAngle"), LightComp->InnerConeAngle);
}

void ASW_CreatorSpotLight::SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject)
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

	LightAttenuationRadius = SetupJsonObject->GetNumberField(TEXT("AttenuationRadius"));
	LightComp->SetAttenuationRadius(LightAttenuationRadius);

	LightOuterConeAngle = SetupJsonObject->GetNumberField(TEXT("OuterConeAngle"));
	LightComp->SetOuterConeAngle(LightOuterConeAngle);

	LightInnerConeAngle = SetupJsonObject->GetNumberField(TEXT("InnerConeAngle"));
	LightComp->SetInnerConeAngle(LightInnerConeAngle);
}
