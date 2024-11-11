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

	// ��� ���忡 �ִ� ������Ʈ�� �˻��Ͽ� ��ī�̶���Ʈ�� �����´�.
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
	// �߰� ������ �����Ѵ�.
	LightColor.R = SetupJsonObject->GetNumberField(TEXT("ColorR"));
	LightColor.G = SetupJsonObject->GetNumberField(TEXT("ColorG"));
	LightColor.B = SetupJsonObject->GetNumberField(TEXT("ColorB"));
	LightColor.A = SetupJsonObject->GetNumberField(TEXT("ColorA"));
	
	if (LightComp)
	{
		LightComp->SetLightColor(LightColor);
	}
}
