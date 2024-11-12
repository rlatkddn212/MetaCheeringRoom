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

void ASW_CreatorSkyLight::OnChangeProperty(int32 id, UCreatorPropertyBase* CreatorProperty)
{
	Super::OnChangeProperty(id, CreatorProperty);

	if (id == 1)
	{
		UCreatorColorProperty* ColorProperty = Cast<UCreatorColorProperty>(CreatorProperty);
		LightColor = ColorProperty->Value;
		LightComp->SetLightColor(LightColor);
	}
}

TMap<int32, UCreatorPropertyBase*> ASW_CreatorSkyLight::GetPropertyMap()
{
	UCreatorColorProperty* Property = NewObject<UCreatorColorProperty>();
	Property->PropertyName = TEXT("Color");
	Property->Value = LightColor;
	AddProperty(1, Property);

	return PropertyMap;
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
	
	if (LightComp)
	{
		LightComp->SetLightColor(LightColor);
	}
}
