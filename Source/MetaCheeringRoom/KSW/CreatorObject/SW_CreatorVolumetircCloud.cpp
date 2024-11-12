// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorObject/SW_CreatorVolumetircCloud.h"
#include "Components/VolumetricCloudComponent.h"

#include "EngineUtils.h"
ASW_CreatorVolumetircCloud::ASW_CreatorVolumetircCloud()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASW_CreatorVolumetircCloud::BeginPlay()
{
	Super::BeginPlay();

	// VolumetricCloud 를 월드에서 가져온다.
	for (TActorIterator<AVolumetricCloud> It(GetWorld()); It; ++It)
	{
		AVolumetricCloud* VolumetricCloudActor = *It;
		if (VolumetricCloudActor)
		{
			VolumetricCloudComp = VolumetricCloudActor->FindComponentByClass<UVolumetricCloudComponent>();
			break;
		}
	}

	if (VolumetricCloudComp)
	{
		LayerBottomAltitude = VolumetricCloudComp->LayerBottomAltitude;
		LayerHeight = VolumetricCloudComp->LayerHeight;
		TracingStartMaxDistance = VolumetricCloudComp->TracingStartMaxDistance;
		TracingStartDistanceFromCamera = VolumetricCloudComp->TracingStartDistanceFromCamera;
		TracingMaxDistance = VolumetricCloudComp->TracingMaxDistance;

		bUsePerSampleAtmosphericLightTransmittance = VolumetricCloudComp->bUsePerSampleAtmosphericLightTransmittance;
		SkyLightCloudBottomOcclusion = VolumetricCloudComp->SkyLightCloudBottomOcclusion;
		ViewSampleCountScale = VolumetricCloudComp->ViewSampleCountScale;
		ReflectionViewSampleCountScaleValue = VolumetricCloudComp->ReflectionViewSampleCountScaleValue;
		ShadowViewSampleCountScale = VolumetricCloudComp->ShadowViewSampleCountScale;
		ShadowReflectionViewSampleCountScaleValue = VolumetricCloudComp->ShadowReflectionViewSampleCountScaleValue;
		ShadowTracingDistance = VolumetricCloudComp->ShadowTracingDistance;
		StopTracingTransmittanceThreshold = VolumetricCloudComp->StopTracingTransmittanceThreshold;

		AerialPespectiveRayleighScatteringStartDistance = VolumetricCloudComp->AerialPespectiveRayleighScatteringStartDistance;
		AerialPespectiveRayleighScatteringFadeDistance = VolumetricCloudComp->AerialPespectiveRayleighScatteringFadeDistance;
		AerialPespectiveMieScatteringStartDistance = VolumetricCloudComp->AerialPespectiveMieScatteringStartDistance;
		AerialPespectiveMieScatteringFadeDistance = VolumetricCloudComp->AerialPespectiveMieScatteringFadeDistance;
	}
}

void ASW_CreatorVolumetircCloud::OnChangeProperty(int32 id, UCreatorPropertyBase* CreatorProperty)
{
	Super::OnChangeProperty(id, CreatorProperty);

	if (id == 1)
	{
		UCreatorFloatProperty* LayerBottomAltitudeProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		LayerBottomAltitude = LayerBottomAltitudeProperty->Value;
		VolumetricCloudComp->LayerBottomAltitude = LayerBottomAltitude;
	}

	if (id == 2)
	{
		UCreatorFloatProperty* LayerHeightProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		LayerHeight = LayerHeightProperty->Value;
		VolumetricCloudComp->LayerHeight = LayerHeight;
	}

	if (id == 3)
	{
		UCreatorFloatProperty* TracingStartMaxDistanceProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		TracingStartMaxDistance = TracingStartMaxDistanceProperty->Value;
		VolumetricCloudComp->TracingStartMaxDistance = TracingStartMaxDistance;
	}

	if (id == 4)
	{
		UCreatorFloatProperty* TracingStartDistanceFromCameraProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		TracingStartDistanceFromCamera = TracingStartDistanceFromCameraProperty->Value;
		VolumetricCloudComp->TracingStartDistanceFromCamera = TracingStartDistanceFromCamera;
	}

	if (id == 5)
	{
		UCreatorFloatProperty* TracingMaxDistanceProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		TracingMaxDistance = TracingMaxDistanceProperty->Value;
		VolumetricCloudComp->TracingMaxDistance = TracingMaxDistance;
	}

	if (id == 6)
	{
		UCreatorBoolProperty* bUsePerSampleAtmosphericLightTransmittanceProperty = Cast<UCreatorBoolProperty>(CreatorProperty);
		bUsePerSampleAtmosphericLightTransmittance = bUsePerSampleAtmosphericLightTransmittanceProperty->Value;
		VolumetricCloudComp->bUsePerSampleAtmosphericLightTransmittance = bUsePerSampleAtmosphericLightTransmittance;
	}
	if (id == 7)
	{
		UCreatorFloatProperty* SkyLightCloudBottomOcclusionProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		SkyLightCloudBottomOcclusion = SkyLightCloudBottomOcclusionProperty->Value;
		VolumetricCloudComp->SkyLightCloudBottomOcclusion = SkyLightCloudBottomOcclusion;
	}

	if (id == 8)
	{
		UCreatorFloatProperty* ViewSampleCountScaleProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		ViewSampleCountScale = ViewSampleCountScaleProperty->Value;
		VolumetricCloudComp->ViewSampleCountScale = ViewSampleCountScale;
	}

	if (id == 9)
	{
		UCreatorFloatProperty* ReflectionViewSampleCountScaleValueProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		ReflectionViewSampleCountScaleValue = ReflectionViewSampleCountScaleValueProperty->Value;
		VolumetricCloudComp->ReflectionViewSampleCountScaleValue = ReflectionViewSampleCountScaleValue;
	}

	if (id == 10)
	{
		UCreatorFloatProperty* ShadowViewSampleCountScaleProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		ShadowViewSampleCountScale = ShadowViewSampleCountScaleProperty->Value;
		VolumetricCloudComp->ShadowViewSampleCountScale = ShadowViewSampleCountScale;
	}

	if (id == 11)
	{
		UCreatorFloatProperty* ShadowReflectionViewSampleCountScaleValueProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		ShadowReflectionViewSampleCountScaleValue = ShadowReflectionViewSampleCountScaleValueProperty->Value;
		VolumetricCloudComp->ShadowReflectionViewSampleCountScaleValue = ShadowReflectionViewSampleCountScaleValue;
	}

	if (id == 12)
	{
		UCreatorFloatProperty* ShadowTracingDistanceProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		ShadowTracingDistance = ShadowTracingDistanceProperty->Value;
		VolumetricCloudComp->ShadowTracingDistance = ShadowTracingDistance;
	}

	if (id == 13)
	{
		UCreatorFloatProperty* StopTracingTransmittanceThresholdProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		StopTracingTransmittanceThreshold = StopTracingTransmittanceThresholdProperty->Value;
		VolumetricCloudComp->StopTracingTransmittanceThreshold = StopTracingTransmittanceThreshold;
	}

	if (id == 14)
	{
		UCreatorFloatProperty* AerialPespectiveRayleighScatteringStartDistanceProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		AerialPespectiveRayleighScatteringStartDistance = AerialPespectiveRayleighScatteringStartDistanceProperty->Value;
		VolumetricCloudComp->AerialPespectiveRayleighScatteringStartDistance = AerialPespectiveRayleighScatteringStartDistance;
	}

	if (id == 15)
	{
		UCreatorFloatProperty* AerialPespectiveRayleighScatteringFadeDistanceProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		AerialPespectiveRayleighScatteringFadeDistance = AerialPespectiveRayleighScatteringFadeDistanceProperty->Value;
		VolumetricCloudComp->AerialPespectiveRayleighScatteringFadeDistance = AerialPespectiveRayleighScatteringFadeDistance;
	}

	if (id == 16)
	{
		UCreatorFloatProperty* AerialPespectiveMieScatteringStartDistanceProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		AerialPespectiveMieScatteringStartDistance = AerialPespectiveMieScatteringStartDistanceProperty->Value;
		VolumetricCloudComp->AerialPespectiveMieScatteringStartDistance = AerialPespectiveMieScatteringStartDistance;
	}

	if (id == 17)
	{
		UCreatorFloatProperty* AerialPespectiveMieScatteringFadeDistanceProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		AerialPespectiveMieScatteringFadeDistance = AerialPespectiveMieScatteringFadeDistanceProperty->Value;
		VolumetricCloudComp->AerialPespectiveMieScatteringFadeDistance = AerialPespectiveMieScatteringFadeDistance;
	}

	VolumetricCloudComp->MarkRenderStateDirty(); // 렌더 상태를 업데이트
}

TMap<int32, UCreatorPropertyBase*> ASW_CreatorVolumetircCloud::GetPropertyMap()
{
	UCreatorFloatProperty* Property = NewObject<UCreatorFloatProperty>();
	Property->PropertyName = TEXT("LayerBottomAltitude");
	Property->Value = LayerBottomAltitude;
	AddProperty(1, Property);

	UCreatorFloatProperty* Property2 = NewObject<UCreatorFloatProperty>();
	Property2->PropertyName = TEXT("LayerHeight");
	Property2->Value = LayerHeight;
	AddProperty(2, Property2);

	UCreatorFloatProperty* Property3 = NewObject<UCreatorFloatProperty>();
	Property3->PropertyName = TEXT("TracingStartMaxDistance");
	Property3->Value = TracingStartMaxDistance;
	AddProperty(3, Property3);

	UCreatorFloatProperty* Property4 = NewObject<UCreatorFloatProperty>();
	Property4->PropertyName = TEXT("TracingStartDistanceFromCamera");
	Property4->Value = TracingStartDistanceFromCamera;
	AddProperty(4, Property4);

	UCreatorFloatProperty* Property5 = NewObject<UCreatorFloatProperty>();
	Property5->PropertyName = TEXT("TracingMaxDistance");
	Property5->Value = TracingMaxDistance;
	AddProperty(5, Property5);

	UCreatorBoolProperty* Property6 = NewObject<UCreatorBoolProperty>();
	Property6->PropertyName = TEXT("bUsePerSampleAtmosphericLightTransmittance");
	Property6->Value = bUsePerSampleAtmosphericLightTransmittance;
	AddProperty(6, Property6);

	UCreatorFloatProperty* Property7 = NewObject<UCreatorFloatProperty>();
	Property7->PropertyName = TEXT("SkyLightCloudBottomOcclusion");
	Property7->Value = SkyLightCloudBottomOcclusion;
	AddProperty(7, Property7);

	UCreatorFloatProperty* Property8 = NewObject<UCreatorFloatProperty>();
	Property8->PropertyName = TEXT("ViewSampleCountScale");
	Property8->Value = ViewSampleCountScale;
	AddProperty(8, Property8);

	UCreatorFloatProperty* Property9 = NewObject<UCreatorFloatProperty>();
	Property9->PropertyName = TEXT("ReflectionViewSampleCountScaleValue");
	Property9->Value = ReflectionViewSampleCountScaleValue;
	AddProperty(9, Property9);

	UCreatorFloatProperty* Property10 = NewObject<UCreatorFloatProperty>();
	Property10->PropertyName = TEXT("ShadowViewSampleCountScale");
	Property10->Value = ShadowViewSampleCountScale;
	AddProperty(10, Property10);

	UCreatorFloatProperty* Property11 = NewObject<UCreatorFloatProperty>();
	Property11->PropertyName = TEXT("ShadowReflectionViewSampleCountScaleValue");
	Property11->Value = ShadowReflectionViewSampleCountScaleValue;
	AddProperty(11, Property11);

	UCreatorFloatProperty* Property12 = NewObject<UCreatorFloatProperty>();
	Property12->PropertyName = TEXT("ShadowTracingDistance");
	Property12->Value = ShadowTracingDistance;
	AddProperty(12, Property12);

	UCreatorFloatProperty* Property13 = NewObject<UCreatorFloatProperty>();
	Property13->PropertyName = TEXT("StopTracingTransmittanceThreshold");
	Property13->Value = StopTracingTransmittanceThreshold;
	AddProperty(13, Property13);

	UCreatorFloatProperty* Property14 = NewObject<UCreatorFloatProperty>();
	Property14->PropertyName = TEXT("AerialPespectiveRayleighScatteringStartDistance");
	Property14->Value = AerialPespectiveRayleighScatteringStartDistance;
	AddProperty(14, Property14);

	UCreatorFloatProperty* Property15 = NewObject<UCreatorFloatProperty>();
	Property15->PropertyName = TEXT("AerialPespectiveRayleighScatteringFadeDistance");
	Property15->Value = AerialPespectiveRayleighScatteringFadeDistance;
	AddProperty(15, Property15);

	UCreatorFloatProperty* Property16 = NewObject<UCreatorFloatProperty>();
	Property16->PropertyName = TEXT("AerialPespectiveMieScatteringStartDistance");
	Property16->Value = AerialPespectiveMieScatteringStartDistance;
	AddProperty(16, Property16);

	UCreatorFloatProperty* Property17 = NewObject<UCreatorFloatProperty>();
	Property17->PropertyName = TEXT("AerialPespectiveMieScatteringFadeDistance");
	Property17->Value = AerialPespectiveMieScatteringFadeDistance;
	AddProperty(17, Property17);

	return PropertyMap;

}

void ASW_CreatorVolumetircCloud::RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const
{
	Super::RecordJsonAdditionalInfo(RecordJsonObject);
	RecordJsonObject->SetNumberField(TEXT("LayerBottomAltitude"), LayerBottomAltitude);
	RecordJsonObject->SetNumberField(TEXT("LayerHeight"), LayerHeight);
	RecordJsonObject->SetNumberField(TEXT("TracingStartMaxDistance"), TracingStartMaxDistance);
	RecordJsonObject->SetNumberField(TEXT("TracingStartDistanceFromCamera"), TracingStartDistanceFromCamera);
	RecordJsonObject->SetNumberField(TEXT("TracingMaxDistance"), TracingMaxDistance);
	RecordJsonObject->SetBoolField(TEXT("bUsePerSampleAtmosphericLightTransmittance"), bUsePerSampleAtmosphericLightTransmittance);
	RecordJsonObject->SetNumberField(TEXT("SkyLightCloudBottomOcclusion"), SkyLightCloudBottomOcclusion);
	RecordJsonObject->SetNumberField(TEXT("ViewSampleCountScale"), ViewSampleCountScale);
	RecordJsonObject->SetNumberField(TEXT("ReflectionViewSampleCountScaleValue"), ReflectionViewSampleCountScaleValue);
	RecordJsonObject->SetNumberField(TEXT("ShadowViewSampleCountScale"), ShadowViewSampleCountScale);
	RecordJsonObject->SetNumberField(TEXT("ShadowReflectionViewSampleCountScaleValue"), ShadowReflectionViewSampleCountScaleValue);
	RecordJsonObject->SetNumberField(TEXT("ShadowTracingDistance"), ShadowTracingDistance);
	RecordJsonObject->SetNumberField(TEXT("StopTracingTransmittanceThreshold"), StopTracingTransmittanceThreshold);
	RecordJsonObject->SetNumberField(TEXT("AerialPespectiveRayleighScatteringStartDistance"), AerialPespectiveRayleighScatteringStartDistance);
	RecordJsonObject->SetNumberField(TEXT("AerialPespectiveRayleighScatteringFadeDistance"), AerialPespectiveRayleighScatteringFadeDistance);
	RecordJsonObject->SetNumberField(TEXT("AerialPespectiveMieScatteringStartDistance"), AerialPespectiveMieScatteringStartDistance);
	RecordJsonObject->SetNumberField(TEXT("AerialPespectiveMieScatteringFadeDistance"), AerialPespectiveMieScatteringFadeDistance);
}

void ASW_CreatorVolumetircCloud::SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject)
{
	Super::SetupJsonAdditionalInfo(SetupJsonObject);
	// 추가 정보를 설정한다.
	LayerBottomAltitude = SetupJsonObject->GetNumberField(TEXT("LayerBottomAltitude"));
	LayerHeight = SetupJsonObject->GetNumberField(TEXT("LayerHeight"));
	TracingStartMaxDistance = SetupJsonObject->GetNumberField(TEXT("TracingStartMaxDistance"));
	TracingStartDistanceFromCamera = SetupJsonObject->GetNumberField(TEXT("TracingStartDistanceFromCamera"));
	TracingMaxDistance = SetupJsonObject->GetNumberField(TEXT("TracingMaxDistance"));
	bUsePerSampleAtmosphericLightTransmittance = SetupJsonObject->GetBoolField(TEXT("bUsePerSampleAtmosphericLightTransmittance"));
	SkyLightCloudBottomOcclusion = SetupJsonObject->GetNumberField(TEXT("SkyLightCloudBottomOcclusion"));
	ViewSampleCountScale = SetupJsonObject->GetNumberField(TEXT("ViewSampleCountScale"));
	ReflectionViewSampleCountScaleValue = SetupJsonObject->GetNumberField(TEXT("ReflectionViewSampleCountScaleValue"));
	ShadowViewSampleCountScale = SetupJsonObject->GetNumberField(TEXT("ShadowViewSampleCountScale"));
	ShadowReflectionViewSampleCountScaleValue = SetupJsonObject->GetNumberField(TEXT("ShadowReflectionViewSampleCountScaleValue"));
	ShadowTracingDistance = SetupJsonObject->GetNumberField(TEXT("ShadowTracingDistance"));
	StopTracingTransmittanceThreshold = SetupJsonObject->GetNumberField(TEXT("StopTracingTransmittanceThreshold"));
	AerialPespectiveRayleighScatteringStartDistance = SetupJsonObject->GetNumberField(TEXT("AerialPespectiveRayleighScatteringStartDistance"));
	AerialPespectiveRayleighScatteringFadeDistance = SetupJsonObject->GetNumberField(TEXT("AerialPespectiveRayleighScatteringFadeDistance"));
	AerialPespectiveMieScatteringStartDistance = SetupJsonObject->GetNumberField(TEXT("AerialPespectiveMieScatteringStartDistance"));
	AerialPespectiveMieScatteringFadeDistance = SetupJsonObject->GetNumberField(TEXT("AerialPespectiveMieScatteringFadeDistance"));

	if (VolumetricCloudComp)
	{
		VolumetricCloudComp->LayerBottomAltitude = LayerBottomAltitude;
		VolumetricCloudComp->LayerHeight = LayerHeight;
		VolumetricCloudComp->TracingStartMaxDistance = TracingStartMaxDistance;
		VolumetricCloudComp->TracingStartDistanceFromCamera = TracingStartDistanceFromCamera;
		VolumetricCloudComp->TracingMaxDistance = TracingMaxDistance;
		VolumetricCloudComp->bUsePerSampleAtmosphericLightTransmittance = bUsePerSampleAtmosphericLightTransmittance;
		VolumetricCloudComp->SkyLightCloudBottomOcclusion = SkyLightCloudBottomOcclusion;
		VolumetricCloudComp->ViewSampleCountScale = ViewSampleCountScale;
		VolumetricCloudComp->ReflectionViewSampleCountScaleValue = ReflectionViewSampleCountScaleValue;
		VolumetricCloudComp->ShadowViewSampleCountScale = ShadowViewSampleCountScale;
		VolumetricCloudComp->ShadowReflectionViewSampleCountScaleValue = ShadowReflectionViewSampleCountScaleValue;
		VolumetricCloudComp->ShadowTracingDistance = ShadowTracingDistance;
		VolumetricCloudComp->StopTracingTransmittanceThreshold = StopTracingTransmittanceThreshold;
		VolumetricCloudComp->AerialPespectiveRayleighScatteringStartDistance = AerialPespectiveRayleighScatteringStartDistance;
		VolumetricCloudComp->AerialPespectiveRayleighScatteringFadeDistance = AerialPespectiveRayleighScatteringFadeDistance;
		VolumetricCloudComp->AerialPespectiveMieScatteringStartDistance = AerialPespectiveMieScatteringStartDistance;
		VolumetricCloudComp->AerialPespectiveMieScatteringFadeDistance = AerialPespectiveMieScatteringFadeDistance;
	}
}
