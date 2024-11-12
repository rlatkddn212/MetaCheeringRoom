// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorObject/SW_CreatorExponentialHeightFog.h"
#include "Engine/ExponentialHeightFog.h"
#include "EngineUtils.h"
#include "Components/ExponentialHeightFogComponent.h"

ASW_CreatorExponentialHeightFog::ASW_CreatorExponentialHeightFog()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASW_CreatorExponentialHeightFog::BeginPlay()
{
	Super::BeginPlay();

	// 월드에서 ExponentialHeightFog를 찾아서 설정한다.
	for (TActorIterator<AExponentialHeightFog> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ExponentialHeightFogComp = ActorItr->GetComponent();
		break;
	}

	if (ExponentialHeightFogComp)
	{
		FogDensity = ExponentialHeightFogComp->FogDensity;
		FogHeightFalloff = ExponentialHeightFogComp->FogHeightFalloff;
		FogInscatteringLuminance = ExponentialHeightFogComp->FogInscatteringLuminance;
		DirectionalInscatteringExponent = ExponentialHeightFogComp->DirectionalInscatteringExponent;
		DirectionalInscatteringStartDistance = ExponentialHeightFogComp->DirectionalInscatteringStartDistance;
		bEnableVolumetricFog = ExponentialHeightFogComp->bEnableVolumetricFog;
		VolumetricFogScatteringDistribution = ExponentialHeightFogComp->VolumetricFogScatteringDistribution;
		VolumetricFogExtinctionScale = ExponentialHeightFogComp->VolumetricFogExtinctionScale;
	}
}

void ASW_CreatorExponentialHeightFog::OnChangeProperty(int32 id, UCreatorPropertyBase* CreatorProperty)
{
	Super::OnChangeProperty(id, CreatorProperty);

	if (id == 1)
	{
		UCreatorFloatProperty* FloatProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		FogDensity = FloatProperty->Value;
		ExponentialHeightFogComp->SetFogDensity(FogDensity);
	}

	if (id == 2)
	{
		UCreatorFloatProperty* FloatProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		FogHeightFalloff = FloatProperty->Value;
		ExponentialHeightFogComp->SetFogHeightFalloff(FogHeightFalloff);
	}

	if (id == 3)
	{
		UCreatorColorProperty* ColorProperty = Cast<UCreatorColorProperty>(CreatorProperty);
		FogInscatteringLuminance = ColorProperty->Value;
		ExponentialHeightFogComp->SetFogInscatteringColor(FogInscatteringLuminance);
	}

	if (id == 4)
	{
		UCreatorFloatProperty* FloatProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		DirectionalInscatteringExponent = FloatProperty->Value;
		ExponentialHeightFogComp->SetDirectionalInscatteringExponent(DirectionalInscatteringExponent);
	}

	if (id == 5)
	{
		UCreatorFloatProperty* FloatProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		DirectionalInscatteringStartDistance = FloatProperty->Value;
		ExponentialHeightFogComp->SetDirectionalInscatteringStartDistance(DirectionalInscatteringStartDistance);
	}

	if (id == 6)
	{
		UCreatorBoolProperty* BoolProperty = Cast<UCreatorBoolProperty>(CreatorProperty);
		bEnableVolumetricFog = BoolProperty->Value;
		ExponentialHeightFogComp->bEnableVolumetricFog = bEnableVolumetricFog;
	}

	if (id == 7)
	{
		UCreatorFloatProperty* FloatProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		VolumetricFogScatteringDistribution = FloatProperty->Value;
		ExponentialHeightFogComp->VolumetricFogScatteringDistribution = VolumetricFogScatteringDistribution;
	}

	if (id == 8)
	{
		UCreatorFloatProperty* FloatProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		VolumetricFogExtinctionScale = FloatProperty->Value;
		ExponentialHeightFogComp->VolumetricFogExtinctionScale = VolumetricFogExtinctionScale;
	}
}

TMap<int32, UCreatorPropertyBase*> ASW_CreatorExponentialHeightFog::GetPropertyMap()
{
	UCreatorFloatProperty* FloatProperty1 = NewObject<UCreatorFloatProperty>();
	FloatProperty1->PropertyName = TEXT("FogDensity");
	FloatProperty1->Value = FogDensity;
	PropertyMap.Add(1, FloatProperty1);

	UCreatorFloatProperty* FloatProperty2 = NewObject<UCreatorFloatProperty>();
	FloatProperty2->PropertyName = TEXT("FogHeightFalloff");
	FloatProperty2->Value = FogHeightFalloff;
	PropertyMap.Add(2, FloatProperty2);

	UCreatorColorProperty* ColorProperty = NewObject<UCreatorColorProperty>();
	ColorProperty->PropertyName = TEXT("FogInscatteringLuminance");
	ColorProperty->Value = FogInscatteringLuminance;
	PropertyMap.Add(3, ColorProperty);

	UCreatorFloatProperty* FloatProperty4 = NewObject<UCreatorFloatProperty>();
	FloatProperty4->PropertyName = TEXT("DirectionalInscatteringExponent");
	FloatProperty4->Value = DirectionalInscatteringExponent;
	PropertyMap.Add(4, FloatProperty4);

	UCreatorFloatProperty* FloatProperty5 = NewObject<UCreatorFloatProperty>();
	FloatProperty5->PropertyName = TEXT("DirectionalInscatteringStartDistance");
	FloatProperty5->Value = DirectionalInscatteringStartDistance;
	PropertyMap.Add(5, FloatProperty5);

	UCreatorBoolProperty* BoolProperty = NewObject<UCreatorBoolProperty>();
	BoolProperty->PropertyName = TEXT("bEnableVolumetricFog");
	BoolProperty->Value = bEnableVolumetricFog;
	PropertyMap.Add(6, BoolProperty);

	UCreatorFloatProperty* FloatProperty7 = NewObject<UCreatorFloatProperty>();
	FloatProperty7->PropertyName = TEXT("VolumetricFogScatteringDistribution");
	FloatProperty7->Value = VolumetricFogScatteringDistribution;
	PropertyMap.Add(7, FloatProperty7);

	UCreatorFloatProperty* FloatProperty8 = NewObject<UCreatorFloatProperty>();
	FloatProperty8->PropertyName = TEXT("VolumetricFogExtinctionScale");
	FloatProperty8->Value = VolumetricFogExtinctionScale;
	PropertyMap.Add(8, FloatProperty8);

	return PropertyMap;
}

void ASW_CreatorExponentialHeightFog::RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const
{
	RecordJsonObject->SetNumberField(TEXT("FogDensity"), FogDensity);
	RecordJsonObject->SetNumberField(TEXT("FogHeightFalloff"), FogHeightFalloff);
	RecordJsonObject->SetStringField(TEXT("FogInscatteringLuminance"), FogInscatteringLuminance.ToString());
	RecordJsonObject->SetNumberField(TEXT("DirectionalInscatteringExponent"), DirectionalInscatteringExponent);
	RecordJsonObject->SetNumberField(TEXT("DirectionalInscatteringStartDistance"), DirectionalInscatteringStartDistance);
	RecordJsonObject->SetBoolField(TEXT("bEnableVolumetricFog"), bEnableVolumetricFog);
	RecordJsonObject->SetNumberField(TEXT("VolumetricFogScatteringDistribution"), VolumetricFogScatteringDistribution);
	RecordJsonObject->SetNumberField(TEXT("VolumetricFogExtinctionScale"), VolumetricFogExtinctionScale);
}

void ASW_CreatorExponentialHeightFog::SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject)
{
	FogDensity = SetupJsonObject->GetNumberField(TEXT("FogDensity"));
	FogHeightFalloff = SetupJsonObject->GetNumberField(TEXT("FogHeightFalloff"));
	FogInscatteringLuminance.InitFromString(SetupJsonObject->GetStringField(TEXT("FogInscatteringLuminance")));
	DirectionalInscatteringExponent = SetupJsonObject->GetNumberField(TEXT("DirectionalInscatteringExponent"));
	DirectionalInscatteringStartDistance = SetupJsonObject->GetNumberField(TEXT("DirectionalInscatteringStartDistance"));
	bEnableVolumetricFog = SetupJsonObject->GetBoolField(TEXT("bEnableVolumetricFog"));
	VolumetricFogScatteringDistribution = SetupJsonObject->GetNumberField(TEXT("VolumetricFogScatteringDistribution"));
	VolumetricFogExtinctionScale = SetupJsonObject->GetNumberField(TEXT("VolumetricFogExtinctionScale"));

	ExponentialHeightFogComp->SetFogDensity(FogDensity);
	ExponentialHeightFogComp->SetFogHeightFalloff(FogHeightFalloff);
	ExponentialHeightFogComp->SetFogInscatteringColor(FogInscatteringLuminance);
	ExponentialHeightFogComp->SetDirectionalInscatteringExponent(DirectionalInscatteringExponent);
	ExponentialHeightFogComp->SetDirectionalInscatteringStartDistance(DirectionalInscatteringStartDistance);
	ExponentialHeightFogComp->bEnableVolumetricFog = bEnableVolumetricFog;
	ExponentialHeightFogComp->VolumetricFogScatteringDistribution = VolumetricFogScatteringDistribution;
	ExponentialHeightFogComp->VolumetricFogExtinctionScale = VolumetricFogExtinctionScale;
}
