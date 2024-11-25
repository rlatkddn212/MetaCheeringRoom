// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorObject/SW_CreatorPointLight.h"
#include "Components/PointLightComponent.h"
#include "Net/UnrealNetwork.h"

ASW_CreatorPointLight::ASW_CreatorPointLight()
{
	PrimaryActorTick.bCanEverTick = true;

	LightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightComp"));
	LightComp->SetupAttachment(Root);

	LightColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void ASW_CreatorPointLight::BeginPlay()
{
	Super::BeginPlay();
	
	if (!HasAuthority())
	{
		OnRep_LightColorChanged();
		OnRep_LightIntensityChanged();
		OnRep_LightAttenuationRadiusChanged();
		OnRep_LightSourceRadiusChanged();
		OnRep_LightCastShadowChanged();
	}
}

void ASW_CreatorPointLight::OnChangeProperty(int32 id, UCreatorPropertyBase* CreatorProperty)
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
		LightIntensity = IntensityProperty->Value;
		LightComp->SetIntensity(IntensityProperty->Value);
	}

	if (id == 3)
	{
		UCreatorFloatProperty* AttenuationRadiusProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		LightAttenuationRadius = AttenuationRadiusProperty->Value;
		LightComp->SetAttenuationRadius(AttenuationRadiusProperty->Value);
	}

	if (id == 4)
	{
		UCreatorFloatProperty* SourceRadiusProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		LightSourceRadius = SourceRadiusProperty->Value;
		LightComp->SetSourceRadius(SourceRadiusProperty->Value);
	}

	if (id == 5)
	{
		UCreatorBoolProperty* CastShadowProperty = Cast<UCreatorBoolProperty>(CreatorProperty);
		CastShadow = CastShadowProperty->Value;
		LightComp->SetCastShadows(CastShadowProperty->Value);
	}
}

TMap<int32, UCreatorPropertyBase*> ASW_CreatorPointLight::GetPropertyMap()
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
	Property4->PropertyName = TEXT("SourceRadius");
	Property4->Value = LightComp->SourceRadius;
	AddProperty(4, Property4);

	UCreatorBoolProperty* Property5 = NewObject<UCreatorBoolProperty>();
	Property5->PropertyName = TEXT("CastShadow");
	Property5->Value = LightComp->CastShadows;
	AddProperty(5, Property5);

	return PropertyMap;
}

void ASW_CreatorPointLight::RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const
{
	Super::RecordJsonAdditionalInfo(RecordJsonObject);
	RecordJsonObject->SetNumberField(TEXT("ColorR"), LightColor.R);
	RecordJsonObject->SetNumberField(TEXT("ColorG"), LightColor.G);
	RecordJsonObject->SetNumberField(TEXT("ColorB"), LightColor.B);
	RecordJsonObject->SetNumberField(TEXT("ColorA"), LightColor.A);

	RecordJsonObject->SetNumberField(TEXT("Intensity"), LightComp->Intensity);
	RecordJsonObject->SetNumberField(TEXT("AttenuationRadius"), LightComp->AttenuationRadius);
	RecordJsonObject->SetNumberField(TEXT("SourceRadius"), LightComp->SourceRadius);
	RecordJsonObject->SetBoolField(TEXT("CastShadow"), LightComp->CastShadows);
}

void ASW_CreatorPointLight::SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject)
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

	LightSourceRadius = SetupJsonObject->GetNumberField(TEXT("SourceRadius"));
	LightComp->SetSourceRadius(LightSourceRadius);

	CastShadow = SetupJsonObject->GetBoolField(TEXT("CastShadow"));
	LightComp->SetCastShadows(CastShadow);
}

void ASW_CreatorPointLight::OnRep_LightColorChanged()
{
	LightComp->SetLightColor(LightColor);
}

void ASW_CreatorPointLight::OnRep_LightIntensityChanged()
{
	LightComp->SetIntensity(LightIntensity);
}

void ASW_CreatorPointLight::OnRep_LightAttenuationRadiusChanged()
{
	LightComp->SetAttenuationRadius(LightAttenuationRadius);
}

void ASW_CreatorPointLight::OnRep_LightSourceRadiusChanged()
{
	LightComp->SetSourceRadius(LightSourceRadius);
}

void ASW_CreatorPointLight::OnRep_LightCastShadowChanged()
{
	LightComp->SetCastShadows(CastShadow);
}

void ASW_CreatorPointLight::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASW_CreatorPointLight, LightColor);
	DOREPLIFETIME(ASW_CreatorPointLight, LightIntensity);
	DOREPLIFETIME(ASW_CreatorPointLight, LightAttenuationRadius);
	DOREPLIFETIME(ASW_CreatorPointLight, LightSourceRadius);
	DOREPLIFETIME(ASW_CreatorPointLight, CastShadow);
}
