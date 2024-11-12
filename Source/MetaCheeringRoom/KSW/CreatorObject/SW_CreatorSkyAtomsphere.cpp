// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorObject/SW_CreatorSkyAtomsphere.h"
#include "EngineUtils.h"
#include "Components/SkyAtmosphereComponent.h"

ASW_CreatorSkyAtomsphere::ASW_CreatorSkyAtomsphere()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASW_CreatorSkyAtomsphere::BeginPlay()
{
	Super::BeginPlay();

	// 월드에서 SkyAtmosphere를 찾아서 설정한다.
	for (TActorIterator<ASkyAtmosphere> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		SkyAtmosphereComp = ActorItr->GetComponent();
		break;
	}

	if (SkyAtmosphereComp)
	{
		// SkyAtmosphere의 속성을 가져온다.
		AtomsphereHeight = SkyAtmosphereComp->AtmosphereHeight;
		RayleighScatteringScale = SkyAtmosphereComp->RayleighScatteringScale;
		RayleighScattering = SkyAtmosphereComp->RayleighScattering;
		MieScatteringScale = SkyAtmosphereComp->MieScatteringScale;
		MieScattering = SkyAtmosphereComp->MieScattering;
		MieAbsorptionScale = SkyAtmosphereComp->MieAbsorptionScale;
		MieAbsorption = SkyAtmosphereComp->MieAbsorption;
		MieAnisotropy = SkyAtmosphereComp->MieAnisotropy;
	}
}

void ASW_CreatorSkyAtomsphere::OnChangeProperty(int32 id, UCreatorPropertyBase* CreatorProperty)
{
	Super::OnChangeProperty(id, CreatorProperty);

	if (id == 1)
	{
		UCreatorFloatProperty* FloatProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		AtomsphereHeight = FloatProperty->Value;
		SkyAtmosphereComp->SetAtmosphereHeight(AtomsphereHeight);
	}

	if (id == 2)
	{
		UCreatorFloatProperty* FloatProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		RayleighScatteringScale = FloatProperty->Value;
		SkyAtmosphereComp->SetRayleighScatteringScale(RayleighScatteringScale);
	}

	if (id == 3)
	{
		UCreatorColorProperty* ColorProperty = Cast<UCreatorColorProperty>(CreatorProperty);
		RayleighScattering = ColorProperty->Value;
		SkyAtmosphereComp->SetRayleighScattering(RayleighScattering);
	}

	if (id == 4)
	{
		UCreatorFloatProperty* FloatProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		MieScatteringScale = FloatProperty->Value;
		SkyAtmosphereComp->SetMieScatteringScale(MieScatteringScale);
	}

	if (id == 5)
	{
		UCreatorColorProperty* ColorProperty = Cast<UCreatorColorProperty>(CreatorProperty);
		MieScattering = ColorProperty->Value;
		SkyAtmosphereComp->SetMieScattering(MieScattering);
	}

	if (id == 6)
	{
		UCreatorFloatProperty* FloatProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		MieAbsorptionScale = FloatProperty->Value;
		SkyAtmosphereComp->SetMieAbsorptionScale(MieAbsorptionScale);
	}

	if (id == 7)
	{
		UCreatorColorProperty* ColorProperty = Cast<UCreatorColorProperty>(CreatorProperty);
		MieAbsorption = ColorProperty->Value;
		SkyAtmosphereComp->SetMieAbsorption(MieAbsorption);
	}

	if (id == 8)
	{
		UCreatorFloatProperty* FloatProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		MieAnisotropy = FloatProperty->Value;
		SkyAtmosphereComp->SetMieAnisotropy(MieAnisotropy);
	}
}

TMap<int32, UCreatorPropertyBase*> ASW_CreatorSkyAtomsphere::GetPropertyMap()
{
	UCreatorFloatProperty* Property = NewObject<UCreatorFloatProperty>();
	Property->PropertyName = TEXT("AtomsphereHeight");
	Property->Value = AtomsphereHeight;
	AddProperty(1, Property);

	UCreatorFloatProperty* Property2 = NewObject<UCreatorFloatProperty>();
	Property2->PropertyName = TEXT("RayleighScatteringScale");
	Property2->Value = RayleighScatteringScale;
	AddProperty(2, Property2);

	UCreatorColorProperty* Property3 = NewObject<UCreatorColorProperty>();
	Property3->PropertyName = TEXT("RayleighScattering");
	Property3->Value = RayleighScattering;
	AddProperty(3, Property3);

	UCreatorFloatProperty* Property4 = NewObject<UCreatorFloatProperty>();
	Property4->PropertyName = TEXT("MieScatteringScale");
	Property4->Value = MieScatteringScale;
	AddProperty(4, Property4);

	UCreatorColorProperty* Property5 = NewObject<UCreatorColorProperty>();
	Property5->PropertyName = TEXT("MieScattering");
	Property5->Value = MieScattering;
	AddProperty(5, Property5);

	UCreatorFloatProperty* Property6 = NewObject<UCreatorFloatProperty>();
	Property6->PropertyName = TEXT("MieAbsorptionScale");
	Property6->Value = MieAbsorptionScale;
	AddProperty(6, Property6);

	UCreatorColorProperty* Property7 = NewObject<UCreatorColorProperty>();
	Property7->PropertyName = TEXT("MieAbsorption");
	Property7->Value = MieAbsorption;
	AddProperty(7, Property7);

	UCreatorFloatProperty* Property8 = NewObject<UCreatorFloatProperty>();
	Property8->PropertyName = TEXT("MieAnisotropy");
	Property8->Value = MieAnisotropy;
	AddProperty(8, Property8);

	return PropertyMap;
}

void ASW_CreatorSkyAtomsphere::RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const
{
	Super::RecordJsonAdditionalInfo(RecordJsonObject);
	RecordJsonObject->SetNumberField(TEXT("AtomsphereHeight"), AtomsphereHeight);
	RecordJsonObject->SetNumberField(TEXT("RayleighScatteringScale"), RayleighScatteringScale);
	RecordJsonObject->SetStringField(TEXT("RayleighScattering"), RayleighScattering.ToString());
	RecordJsonObject->SetNumberField(TEXT("MieScatteringScale"), MieScatteringScale);
	RecordJsonObject->SetStringField(TEXT("MieScattering"), MieScattering.ToString());
	RecordJsonObject->SetNumberField(TEXT("MieAbsorptionScale"), MieAbsorptionScale);
	RecordJsonObject->SetStringField(TEXT("MieAbsorption"), MieAbsorption.ToString());
	RecordJsonObject->SetNumberField(TEXT("MieAnisotropy"), MieAnisotropy);
}

void ASW_CreatorSkyAtomsphere::SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject)
{
	Super::SetupJsonAdditionalInfo(SetupJsonObject);
	// 추가 정보를 설정한다.
	AtomsphereHeight = SetupJsonObject->GetNumberField(TEXT("AtomsphereHeight"));
	RayleighScatteringScale = SetupJsonObject->GetNumberField(TEXT("RayleighScatteringScale"));
	RayleighScattering.InitFromString(SetupJsonObject->GetStringField(TEXT("RayleighScattering")));
	MieScatteringScale = SetupJsonObject->GetNumberField(TEXT("MieScatteringScale"));
	MieScattering.InitFromString(SetupJsonObject->GetStringField(TEXT("MieScattering")));
	MieAbsorptionScale = SetupJsonObject->GetNumberField(TEXT("MieAbsorptionScale"));
	MieAbsorption.InitFromString(SetupJsonObject->GetStringField(TEXT("MieAbsorption")));
	MieAnisotropy = SetupJsonObject->GetNumberField(TEXT("MieAnisotropy"));

	if (SkyAtmosphereComp)
	{
		SkyAtmosphereComp->SetAtmosphereHeight(AtomsphereHeight);
		SkyAtmosphereComp->SetRayleighScatteringScale(RayleighScatteringScale);
		SkyAtmosphereComp->SetRayleighScattering(RayleighScattering);
		SkyAtmosphereComp->SetMieScatteringScale(MieScatteringScale);
		SkyAtmosphereComp->SetMieScattering(MieScattering);
		SkyAtmosphereComp->SetMieAbsorptionScale(MieAbsorptionScale);
		SkyAtmosphereComp->SetMieAbsorption(MieAbsorption);
		SkyAtmosphereComp->SetMieAnisotropy(MieAnisotropy);
	}
}
