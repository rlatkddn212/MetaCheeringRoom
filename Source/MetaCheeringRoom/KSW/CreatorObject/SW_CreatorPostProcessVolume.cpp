// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorObject/SW_CreatorPostProcessVolume.h"
#include "EngineUtils.h"
#include "Components/PostProcessComponent.h"
#include "Engine/PostProcessVolume.h"

ASW_CreatorPostProcessVolume::ASW_CreatorPostProcessVolume()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASW_CreatorPostProcessVolume::BeginPlay()
{
	Super::BeginPlay();
	
	// ���忡�� PostProcessVolume�� �����´�.
	for (TActorIterator<APostProcessVolume> It(GetWorld()); It; ++It)
	{
		PostProcessActor = *It;
		break;
	}

	if (PostProcessActor)
	{
		// PostProcessComponent�� �������� �����´�.
		bOverride_BloomMethod = PostProcessActor->Settings.bOverride_BloomMethod;
		BloomMethod = PostProcessActor->Settings.BloomMethod;
		bOverride_BloomIntensity = PostProcessActor->Settings.bOverride_BloomIntensity;
		BloomIntensity = PostProcessActor->Settings.BloomIntensity;
		bOverride_BloomThreshold = PostProcessActor->Settings.bOverride_BloomThreshold;
		BloomThreshold = PostProcessActor->Settings.BloomThreshold;
		bOverride_AutoExposureMethod = PostProcessActor->Settings.bOverride_AutoExposureMethod;
		AutoExposureMethod = PostProcessActor->Settings.AutoExposureMethod;
		bOverride_AutoExposureMinBrightness = PostProcessActor->Settings.bOverride_AutoExposureMinBrightness;
		AutoExposureMinBrightness = PostProcessActor->Settings.AutoExposureMinBrightness;
		bOverride_AutoExposureMaxBrightness = PostProcessActor->Settings.bOverride_AutoExposureMaxBrightness;
		AutoExposureMaxBrightness = PostProcessActor->Settings.AutoExposureMaxBrightness;
		bOverride_AutoExposureSpeedUp = PostProcessActor->Settings.bOverride_AutoExposureSpeedUp;
		AutoExposureSpeedUp = PostProcessActor->Settings.AutoExposureSpeedUp;
		bOverride_AutoExposureSpeedDown = PostProcessActor->Settings.bOverride_AutoExposureSpeedDown;
		AutoExposureSpeedDown = PostProcessActor->Settings.AutoExposureSpeedDown;
		bOverride_LensFlareIntensity = PostProcessActor->Settings.bOverride_LensFlareIntensity;
		LensFlareIntensity = PostProcessActor->Settings.LensFlareIntensity;
		bOverride_LensFlareTint = PostProcessActor->Settings.bOverride_LensFlareTint;
		LensFlareTint = PostProcessActor->Settings.LensFlareTint;
		bOverride_LensFlareBokehSize = PostProcessActor->Settings.bOverride_LensFlareBokehSize;
		LensFlareBokehSize = PostProcessActor->Settings.LensFlareBokehSize;
		bOverride_LensFlareThreshold = PostProcessActor->Settings.bOverride_LensFlareThreshold;
		LensFlareThreshold = PostProcessActor->Settings.LensFlareThreshold;
	}
}

void ASW_CreatorPostProcessVolume::OnChangeProperty(int32 id, UCreatorPropertyBase* CreatorProperty)
{
	Super::OnChangeProperty(id, CreatorProperty);
	if (PostProcessActor == nullptr) return;
	if (id == 1)
	{
		// BloomMethod
		UCreatorEnumProperty* Property = Cast<UCreatorEnumProperty>(CreatorProperty);
		bOverride_BloomMethod = Property->bUse;
		PostProcessActor->Settings.bOverride_BloomMethod = bOverride_BloomMethod;
		if (Property->Value == "Standard")
		{
			PostProcessActor->Settings.BloomMethod = EBloomMethod::BM_SOG;
			BloomMethod = EBloomMethod::BM_SOG;
		}
		else if (Property->Value == "Convolution")
		{
			PostProcessActor->Settings.BloomMethod = EBloomMethod::BM_FFT;
			BloomMethod = EBloomMethod::BM_FFT;
		}
	}
	else if (id == 2)
	{
		// BloomIntensity
		UCreatorFloatProperty* Property = Cast<UCreatorFloatProperty>(CreatorProperty);
		bOverride_BloomIntensity = Property->bUse;
		BloomIntensity = Property->Value;
		PostProcessActor->Settings.bOverride_BloomIntensity = bOverride_BloomIntensity;
		PostProcessActor->Settings.BloomIntensity = Property->Value;
	}
	else if (id == 3)
	{
		// BloomThreshold
		UCreatorFloatProperty* Property = Cast<UCreatorFloatProperty>(CreatorProperty);
		bOverride_BloomThreshold = Property->bUse;
		BloomThreshold = Property->Value;
		PostProcessActor->Settings.bOverride_BloomThreshold = bOverride_BloomThreshold;
		PostProcessActor->Settings.BloomThreshold = Property->Value;
	}
	else if (id == 4)
	{
		// AutoExposureMethod
		UCreatorEnumProperty* Property = Cast<UCreatorEnumProperty>(CreatorProperty);
		bOverride_AutoExposureMethod = Property->bUse;
		PostProcessActor->Settings.bOverride_AutoExposureMethod = bOverride_AutoExposureMethod;
		if (Property->Value == "Histogram")
		{
			AutoExposureMethod = EAutoExposureMethod::AEM_Histogram;
			PostProcessActor->Settings.AutoExposureMethod = EAutoExposureMethod::AEM_Histogram;
		}
		else if (Property->Value == "Basic")
		{
			AutoExposureMethod = EAutoExposureMethod::AEM_Basic;
			PostProcessActor->Settings.AutoExposureMethod = EAutoExposureMethod::AEM_Basic;
		}
		else if (Property->Value == "Manual")
		{
			AutoExposureMethod = EAutoExposureMethod::AEM_Manual;
			PostProcessActor->Settings.AutoExposureMethod = EAutoExposureMethod::AEM_Manual;
		}
	}
	else if (id == 5)
	{
		// AutoExposureMinBrightness
		UCreatorFloatProperty* Property = Cast<UCreatorFloatProperty>(CreatorProperty);
		bOverride_AutoExposureMinBrightness = Property->bUse;
		AutoExposureMinBrightness = Property->Value;
		PostProcessActor->Settings.bOverride_AutoExposureMinBrightness = bOverride_AutoExposureMinBrightness;
		PostProcessActor->Settings.AutoExposureMinBrightness = Property->Value;
	}
	else if (id == 6)
	{
		// AutoExposureMaxBrightness
		UCreatorFloatProperty* Property = Cast<UCreatorFloatProperty>(CreatorProperty);
		bOverride_AutoExposureMaxBrightness = Property->bUse;
		AutoExposureMaxBrightness = Property->Value;
		PostProcessActor->Settings.bOverride_AutoExposureMaxBrightness = bOverride_AutoExposureMaxBrightness;
		PostProcessActor->Settings.AutoExposureMaxBrightness = Property->Value;
	}
	else if (id == 7)
	{
		// AutoExposureSpeedUp
		UCreatorFloatProperty* Property = Cast<UCreatorFloatProperty>(CreatorProperty);
		bOverride_AutoExposureSpeedUp = Property->bUse;
		AutoExposureSpeedUp = Property->Value;
		PostProcessActor->Settings.bOverride_AutoExposureSpeedUp = bOverride_AutoExposureSpeedUp;
		PostProcessActor->Settings.AutoExposureSpeedUp = Property->Value;
	}
	else if (id == 8)
	{
		// AutoExposureSpeedDown
		UCreatorFloatProperty* Property = Cast<UCreatorFloatProperty>(CreatorProperty);
		bOverride_AutoExposureSpeedDown = Property->bUse;
		AutoExposureSpeedDown = Property->Value;
		PostProcessActor->Settings.bOverride_AutoExposureSpeedDown = bOverride_AutoExposureSpeedDown;
		PostProcessActor->Settings.AutoExposureSpeedDown = Property->Value;
	}
	else if (id == 9)
	{
		// LensFlareIntensity
		UCreatorFloatProperty* Property = Cast<UCreatorFloatProperty>(CreatorProperty);
		bOverride_LensFlareIntensity = Property->bUse;
		LensFlareIntensity = Property->Value;
		PostProcessActor->Settings.bOverride_LensFlareIntensity = bOverride_LensFlareIntensity;
		PostProcessActor->Settings.LensFlareIntensity = Property->Value;
	}
	else if (id == 10)
	{
		// LensFlareTint
		UCreatorColorProperty* Property = Cast<UCreatorColorProperty>(CreatorProperty);
		bOverride_LensFlareTint = Property->bUse;
		LensFlareTint = Property->Value;
		PostProcessActor->Settings.bOverride_LensFlareTint = bOverride_LensFlareTint;
		PostProcessActor->Settings.LensFlareTint = Property->Value;
	}
	else if (id == 11)
	{
		// LensFlareBokehSize
		UCreatorFloatProperty* Property = Cast<UCreatorFloatProperty>(CreatorProperty);
		bOverride_LensFlareBokehSize = Property->bUse;
		LensFlareBokehSize = Property->Value;
		PostProcessActor->Settings.bOverride_LensFlareBokehSize = bOverride_LensFlareBokehSize;
		PostProcessActor->Settings.LensFlareBokehSize = Property->Value;
	}
	else if (id == 12)
	{
		// LensFlareThreshold
		UCreatorFloatProperty* Property = Cast<UCreatorFloatProperty>(CreatorProperty);
		bOverride_LensFlareThreshold = Property->bUse;
		LensFlareThreshold = Property->Value;
		PostProcessActor->Settings.bOverride_LensFlareThreshold = bOverride_LensFlareThreshold;
		PostProcessActor->Settings.LensFlareThreshold = Property->Value;
	}

}

TMap<int32, UCreatorPropertyBase*> ASW_CreatorPostProcessVolume::GetPropertyMap()
{
	UCreatorEnumProperty* BloomMethodProperty = NewObject<UCreatorEnumProperty>();
	BloomMethodProperty->PropertyName = "BloomMethod";
	BloomMethodProperty->bCheckBox = true;
	BloomMethodProperty->bUse = bOverride_BloomMethod;
	BloomMethodProperty->Value = BloomMethod == EBloomMethod::BM_SOG ? "Standard" : "Convolution";
	BloomMethodProperty->EnumValues.Add("Standard");
	BloomMethodProperty->EnumValues.Add("Convolution");
	PropertyMap.Add(1, BloomMethodProperty);

	UCreatorFloatProperty* BloomIntensityProperty = NewObject<UCreatorFloatProperty>();
	BloomIntensityProperty->PropertyName = "BloomIntensity";
	BloomIntensityProperty->bCheckBox = true;
	BloomIntensityProperty->bUse = bOverride_BloomIntensity;
	BloomIntensityProperty->Value = BloomIntensity;
	PropertyMap.Add(2, BloomIntensityProperty);

	UCreatorFloatProperty* BloomThresholdProperty = NewObject<UCreatorFloatProperty>();
	BloomThresholdProperty->PropertyName = "BloomThreshold";
	BloomThresholdProperty->bCheckBox = true;
	BloomThresholdProperty->bUse = bOverride_BloomThreshold;
	BloomThresholdProperty->Value = BloomThreshold;
	PropertyMap.Add(3, BloomThresholdProperty);
	
	UCreatorEnumProperty* AutoExposureMethodProperty = NewObject<UCreatorEnumProperty>();
	AutoExposureMethodProperty->PropertyName = "AutoExposureMethod";
	AutoExposureMethodProperty->bCheckBox = true;
	AutoExposureMethodProperty->bUse = bOverride_AutoExposureMethod;

	if (AutoExposureMethod == EAutoExposureMethod::AEM_Histogram)
	{
		AutoExposureMethodProperty->Value = "Histogram";
	}
	else if (AutoExposureMethod == EAutoExposureMethod::AEM_Basic)
	{
		AutoExposureMethodProperty->Value = "Basic";
	}
	else if (AutoExposureMethod == EAutoExposureMethod::AEM_Manual)
	{
		AutoExposureMethodProperty->Value = "Manual";
	}

	AutoExposureMethodProperty->EnumValues.Add("Histogram");
	AutoExposureMethodProperty->EnumValues.Add("Basic");
	AutoExposureMethodProperty->EnumValues.Add("Manual");
	PropertyMap.Add(4, AutoExposureMethodProperty);

	UCreatorFloatProperty* AutoExposureMinBrightnessProperty = NewObject<UCreatorFloatProperty>();
	AutoExposureMinBrightnessProperty->PropertyName = "AutoExposureMinBrightness";
	AutoExposureMinBrightnessProperty->bCheckBox = true;
	AutoExposureMinBrightnessProperty->bUse = bOverride_AutoExposureMinBrightness;
	AutoExposureMinBrightnessProperty->Value = AutoExposureMinBrightness;
	PropertyMap.Add(5, AutoExposureMinBrightnessProperty);

	UCreatorFloatProperty* AutoExposureMaxBrightnessProperty = NewObject<UCreatorFloatProperty>();
	AutoExposureMaxBrightnessProperty->PropertyName = "AutoExposureMaxBrightness";
	AutoExposureMaxBrightnessProperty->bCheckBox = true;
	AutoExposureMaxBrightnessProperty->bUse = bOverride_AutoExposureMaxBrightness;
	AutoExposureMaxBrightnessProperty->Value = AutoExposureMaxBrightness;
	PropertyMap.Add(6, AutoExposureMaxBrightnessProperty);

	UCreatorFloatProperty* AutoExposureSpeedUpProperty = NewObject<UCreatorFloatProperty>();
	AutoExposureSpeedUpProperty->PropertyName = "AutoExposureSpeedUp";
	AutoExposureSpeedUpProperty->bCheckBox = true;
	AutoExposureSpeedUpProperty->bUse = bOverride_AutoExposureSpeedUp;
	AutoExposureSpeedUpProperty->Value = AutoExposureSpeedUp;
	PropertyMap.Add(7, AutoExposureSpeedUpProperty);

	UCreatorFloatProperty* AutoExposureSpeedDownProperty = NewObject<UCreatorFloatProperty>();
	AutoExposureSpeedDownProperty->PropertyName = "AutoExposureSpeedDown";
	AutoExposureSpeedDownProperty->bCheckBox = true;
	AutoExposureSpeedDownProperty->bUse = bOverride_AutoExposureSpeedDown;
	AutoExposureSpeedDownProperty->Value = AutoExposureSpeedDown;
	PropertyMap.Add(8, AutoExposureSpeedDownProperty);

	UCreatorFloatProperty* LensFlareIntensityProperty = NewObject<UCreatorFloatProperty>();
	LensFlareIntensityProperty->PropertyName = "LensFlareIntensity";
	LensFlareIntensityProperty->bCheckBox = true;
	LensFlareIntensityProperty->bUse = bOverride_LensFlareIntensity;
	LensFlareIntensityProperty->Value = LensFlareIntensity;
	PropertyMap.Add(9, LensFlareIntensityProperty);

	UCreatorColorProperty* LensFlareTintProperty = NewObject<UCreatorColorProperty>();
	LensFlareTintProperty->PropertyName = "LensFlareTint";
	LensFlareTintProperty->bCheckBox = true;
	LensFlareTintProperty->bUse = bOverride_LensFlareTint;
	LensFlareTintProperty->Value = LensFlareTint;
	PropertyMap.Add(10, LensFlareTintProperty);

	UCreatorFloatProperty* LensFlareBokehSizeProperty = NewObject<UCreatorFloatProperty>();
	LensFlareBokehSizeProperty->PropertyName = "LensFlareBokehSize";
	LensFlareBokehSizeProperty->bCheckBox = true;
	LensFlareBokehSizeProperty->bUse = bOverride_LensFlareBokehSize;
	LensFlareBokehSizeProperty->Value = LensFlareBokehSize;
	PropertyMap.Add(11, LensFlareBokehSizeProperty);

	//LensFlareThreshold
	UCreatorFloatProperty* LensFlareThresholdProperty = NewObject<UCreatorFloatProperty>();
	LensFlareThresholdProperty->PropertyName = "LensFlareThreshold";
	LensFlareThresholdProperty->bCheckBox = true;
	LensFlareThresholdProperty->bUse = bOverride_LensFlareThreshold;
	LensFlareThresholdProperty->Value = LensFlareThreshold;
	PropertyMap.Add(12, LensFlareThresholdProperty);

	return PropertyMap;
}

void ASW_CreatorPostProcessVolume::RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const
{
	RecordJsonObject->SetBoolField(L"bOverride_BloomMethod", bOverride_BloomMethod);
	RecordJsonObject->SetNumberField("BloomMethod", (int32)BloomMethod);
	RecordJsonObject->SetBoolField("bOverride_BloomIntensity", bOverride_BloomIntensity);
	RecordJsonObject->SetNumberField("BloomIntensity", BloomIntensity);
	RecordJsonObject->SetBoolField("bOverride_BloomThreshold", bOverride_BloomThreshold);
	RecordJsonObject->SetNumberField("BloomThreshold", BloomThreshold);
	RecordJsonObject->SetBoolField("bOverride_AutoExposureMethod", bOverride_AutoExposureMethod);
	RecordJsonObject->SetNumberField("AutoExposureMethod", (int32)AutoExposureMethod);
	RecordJsonObject->SetBoolField("bOverride_AutoExposureMinBrightness", bOverride_AutoExposureMinBrightness);
	RecordJsonObject->SetNumberField("AutoExposureMinBrightness", AutoExposureMinBrightness);
	RecordJsonObject->SetBoolField("bOverride_AutoExposureMaxBrightness", bOverride_AutoExposureMaxBrightness);
	RecordJsonObject->SetNumberField("AutoExposureMaxBrightness", AutoExposureMaxBrightness);
	RecordJsonObject->SetBoolField("bOverride_AutoExposureSpeedUp", bOverride_AutoExposureSpeedUp);
	RecordJsonObject->SetNumberField("AutoExposureSpeedUp", AutoExposureSpeedUp);
	RecordJsonObject->SetBoolField("bOverride_AutoExposureSpeedDown", bOverride_AutoExposureSpeedDown);
	RecordJsonObject->SetNumberField("AutoExposureSpeedDown", AutoExposureSpeedDown);
	RecordJsonObject->SetBoolField("bOverride_LensFlareIntensity", bOverride_LensFlareIntensity);
	RecordJsonObject->SetNumberField("LensFlareIntensity", LensFlareIntensity);
	RecordJsonObject->SetBoolField("bOverride_LensFlareTint", bOverride_LensFlareTint);
	RecordJsonObject->SetStringField("LensFlareTint", LensFlareTint.ToString());
	RecordJsonObject->SetBoolField("bOverride_LensFlareBokehSize", bOverride_LensFlareBokehSize);
	RecordJsonObject->SetNumberField("LensFlareBokehSize", LensFlareBokehSize);
	RecordJsonObject->SetBoolField("bOverride_LensFlareThreshold", bOverride_LensFlareThreshold);
	RecordJsonObject->SetNumberField("LensFlareThreshold", LensFlareThreshold);
}

void ASW_CreatorPostProcessVolume::SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject)
{
	SetupJsonObject->TryGetBoolField("bOverride_BloomMethod", bOverride_BloomMethod);
	int32 BloomMethodInt;
	if (SetupJsonObject->TryGetNumberField("BloomMethod", BloomMethodInt))
	{
		BloomMethod = (EBloomMethod)BloomMethodInt;
	}
	SetupJsonObject->TryGetBoolField("bOverride_BloomIntensity", bOverride_BloomIntensity);
	SetupJsonObject->TryGetNumberField("BloomIntensity", BloomIntensity);
	SetupJsonObject->TryGetBoolField("bOverride_BloomThreshold", bOverride_BloomThreshold);
	SetupJsonObject->TryGetNumberField("BloomThreshold", BloomThreshold);
	SetupJsonObject->TryGetBoolField("bOverride_AutoExposureMethod", bOverride_AutoExposureMethod);
	int32 AutoExposureMethodInt;
	if (SetupJsonObject->TryGetNumberField("AutoExposureMethod", AutoExposureMethodInt))
	{
		AutoExposureMethod = (EAutoExposureMethod)AutoExposureMethodInt;
	}
	SetupJsonObject->TryGetBoolField("bOverride_AutoExposureMinBrightness", bOverride_AutoExposureMinBrightness);
	SetupJsonObject->TryGetNumberField("AutoExposureMinBrightness", AutoExposureMinBrightness);
	SetupJsonObject->TryGetBoolField("bOverride_AutoExposureMaxBrightness", bOverride_AutoExposureMaxBrightness);
	SetupJsonObject->TryGetNumberField("AutoExposureMaxBrightness", AutoExposureMaxBrightness);
	SetupJsonObject->TryGetBoolField("bOverride_AutoExposureSpeedUp", bOverride_AutoExposureSpeedUp);
	SetupJsonObject->TryGetNumberField("AutoExposureSpeedUp", AutoExposureSpeedUp);
	SetupJsonObject->TryGetBoolField("bOverride_AutoExposureSpeedDown", bOverride_AutoExposureSpeedDown);
	SetupJsonObject->TryGetNumberField("AutoExposureSpeedDown", AutoExposureSpeedDown);
	SetupJsonObject->TryGetBoolField("bOverride_LensFlareIntensity", bOverride_LensFlareIntensity);
	SetupJsonObject->TryGetNumberField("LensFlareIntensity", LensFlareIntensity);
	SetupJsonObject->TryGetBoolField("bOverride_LensFlareTint", bOverride_LensFlareTint);
	FString LensFlareTintString;
	if (SetupJsonObject->TryGetStringField("LensFlareTint", LensFlareTintString))
	{
		LensFlareTint.InitFromString(LensFlareTintString);
	}

	SetupJsonObject->TryGetBoolField("bOverride_LensFlareBokehSize", bOverride_LensFlareBokehSize);
	SetupJsonObject->TryGetNumberField("LensFlareBokehSize", LensFlareBokehSize);
	SetupJsonObject->TryGetBoolField("bOverride_LensFlareThreshold", bOverride_LensFlareThreshold);
	SetupJsonObject->TryGetNumberField("LensFlareThreshold", LensFlareThreshold);

	// ����
	if (PostProcessActor)
	{
		PostProcessActor->Settings.bOverride_BloomMethod = bOverride_BloomMethod;
		PostProcessActor->Settings.BloomMethod = BloomMethod;
		PostProcessActor->Settings.bOverride_BloomIntensity = bOverride_BloomIntensity;
		PostProcessActor->Settings.BloomIntensity = BloomIntensity;
		PostProcessActor->Settings.bOverride_BloomThreshold = bOverride_BloomThreshold;
		PostProcessActor->Settings.BloomThreshold = BloomThreshold;
		PostProcessActor->Settings.bOverride_AutoExposureMethod = bOverride_AutoExposureMethod;
		PostProcessActor->Settings.AutoExposureMethod = AutoExposureMethod;
		PostProcessActor->Settings.bOverride_AutoExposureMinBrightness = bOverride_AutoExposureMinBrightness;
		PostProcessActor->Settings.AutoExposureMinBrightness = AutoExposureMinBrightness;
		PostProcessActor->Settings.bOverride_AutoExposureMaxBrightness = bOverride_AutoExposureMaxBrightness;
		PostProcessActor->Settings.AutoExposureMaxBrightness = AutoExposureMaxBrightness;
		PostProcessActor->Settings.bOverride_AutoExposureSpeedUp = bOverride_AutoExposureSpeedUp;
		PostProcessActor->Settings.AutoExposureSpeedUp = AutoExposureSpeedUp;
		PostProcessActor->Settings.bOverride_AutoExposureSpeedDown = bOverride_AutoExposureSpeedDown;
		PostProcessActor->Settings.AutoExposureSpeedDown = AutoExposureSpeedDown;
		PostProcessActor->Settings.bOverride_LensFlareIntensity = bOverride_LensFlareIntensity;
		PostProcessActor->Settings.LensFlareIntensity = LensFlareIntensity;
		PostProcessActor->Settings.bOverride_LensFlareTint = bOverride_LensFlareTint;
		PostProcessActor->Settings.LensFlareTint = LensFlareTint;
		PostProcessActor->Settings.bOverride_LensFlareBokehSize = bOverride_LensFlareBokehSize;
		PostProcessActor->Settings.LensFlareBokehSize = LensFlareBokehSize;
		PostProcessActor->Settings.bOverride_LensFlareThreshold = bOverride_LensFlareThreshold;
		PostProcessActor->Settings.LensFlareThreshold = LensFlareThreshold;
	}
}
