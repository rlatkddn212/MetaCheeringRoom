// Fill out your copyright notice in the Description page of Project Settings.


#include "SW_CreatorStaticObject.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "KSW/CreatorGizmo/CreatorPositionGizmoComponent.h"
#include "KSW/CreatorGizmo/CreatorRotationGizmoComponent.h"
#include "KSW/CreatorGizmo/CreatorScaleGizmoComponent.h"
#include "Net/UnrealNetwork.h"

ASW_CreatorStaticObject::ASW_CreatorStaticObject()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	bReplicates = true;
}

void ASW_CreatorStaticObject::BeginPlay()
{
	Super::BeginPlay();
	// 매쉬의 머터리얼을 가져온다.

	Mat = UMaterialInstanceDynamic::Create(Mesh->GetMaterial(0), this);
	Mesh->SetMaterial(0, Mat);

	UCreatorColorProperty* Property = NewObject<UCreatorColorProperty>();
	Property->Value = MeshColor;
	OnRep_MeshColorChanged();

	Property = NewObject<UCreatorColorProperty>();
	Property->Value = EmissiveColor;
	OnRep_EmissiveColorChanged();

	UCreatorFloatProperty* FloatProperty = NewObject<UCreatorFloatProperty>();
	FloatProperty->Value = Metallic;
	OnRep_MeshMetallicChanged();

	FloatProperty = NewObject<UCreatorFloatProperty>();
	FloatProperty->Value = Roughness;
	OnRep_MeshRoughnessChanged();
}

void ASW_CreatorStaticObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASW_CreatorStaticObject::OnSelected(bool isSelected)
{
	Super::OnSelected(isSelected);

	if (isSelected)
	{
		Mesh->SetRenderCustomDepth(true);
		UE_LOG(LogTemp, Warning, TEXT("Selected"));
	}
	else
	{
		Mesh->SetRenderCustomDepth(false);
		UE_LOG(LogTemp, Warning, TEXT("Unselected"));
	}
}

void ASW_CreatorStaticObject::OnChangeProperty(int32 id, UCreatorPropertyBase* CreatorProperty)
{
	Super::OnChangeProperty(id, CreatorProperty);

	if (id == 1)
	{
		UCreatorColorProperty* ColorProperty = Cast<UCreatorColorProperty>(CreatorProperty);
		MeshColor = ColorProperty->Value;

		if (Mat)
		{
			Mat->SetVectorParameterValue("Color", MeshColor);
		}
	}

	if (id == 2)
	{
		UCreatorFloatProperty* FloatProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		Metallic = FloatProperty->Value;

		if (Mat)
		{
			Mat->SetScalarParameterValue("Metallic", Metallic);
		}
	}

	if (id == 3)
	{
		UCreatorFloatProperty* FloatProperty = Cast<UCreatorFloatProperty>(CreatorProperty);
		Roughness = FloatProperty->Value;

		if (Mat)
		{
			Mat->SetScalarParameterValue("Roughness", Roughness);
		}
	}

	if (id == 4)
	{
		UCreatorColorProperty* ColorProperty = Cast<UCreatorColorProperty>(CreatorProperty);
		EmissiveColor = ColorProperty->Value;

		if (Mat)
		{
			Mat->SetVectorParameterValue("EmissiveColor", EmissiveColor);
		}
	}
}

TMap<int32, UCreatorPropertyBase*> ASW_CreatorStaticObject::GetPropertyMap()
{
	UCreatorColorProperty* Property = NewObject<UCreatorColorProperty>();
	Property->PropertyName = TEXT("Color");
	Property->Value = MeshColor;
	AddProperty(1, Property);

	UCreatorFloatProperty* FloatProperty = NewObject<UCreatorFloatProperty>();
	FloatProperty->PropertyName = TEXT("Metallic");
	FloatProperty->Value = Metallic;
	AddProperty(2, FloatProperty);

	FloatProperty = NewObject<UCreatorFloatProperty>();
	FloatProperty->PropertyName = TEXT("Roughness");
	FloatProperty->Value = Roughness;
	AddProperty(3, FloatProperty);

	Property = NewObject<UCreatorColorProperty>();
	Property->PropertyName = TEXT("EmissiveColor");
	Property->Value = EmissiveColor;
	AddProperty(4, Property);

	return PropertyMap;
}

void ASW_CreatorStaticObject::RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const
{
	Super::RecordJsonAdditionalInfo(RecordJsonObject);
	RecordJsonObject->SetNumberField(TEXT("ColorR"), MeshColor.R);
	RecordJsonObject->SetNumberField(TEXT("ColorG"), MeshColor.G);
	RecordJsonObject->SetNumberField(TEXT("ColorB"), MeshColor.B);
	RecordJsonObject->SetNumberField(TEXT("ColorA"), MeshColor.A);

	RecordJsonObject->SetNumberField(TEXT("EmissiveColorR"), EmissiveColor.R);
	RecordJsonObject->SetNumberField(TEXT("EmissiveColorG"), EmissiveColor.G);
	RecordJsonObject->SetNumberField(TEXT("EmissiveColorB"), EmissiveColor.B);
	RecordJsonObject->SetNumberField(TEXT("EmissiveColorA"), EmissiveColor.A);

	RecordJsonObject->SetNumberField(TEXT("Metallic"), Metallic);
	RecordJsonObject->SetNumberField(TEXT("Roughness"), Roughness);
}

void ASW_CreatorStaticObject::SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject)
{
	Super::SetupJsonAdditionalInfo(SetupJsonObject);
	// 추가 정보를 설정한다.
	MeshColor.R = SetupJsonObject->GetNumberField(TEXT("ColorR"));
	MeshColor.G = SetupJsonObject->GetNumberField(TEXT("ColorG"));
	MeshColor.B = SetupJsonObject->GetNumberField(TEXT("ColorB"));
	MeshColor.A = SetupJsonObject->GetNumberField(TEXT("ColorA"));

	EmissiveColor.R = SetupJsonObject->GetNumberField(TEXT("EmissiveColorR"));
	EmissiveColor.G = SetupJsonObject->GetNumberField(TEXT("EmissiveColorG"));
	EmissiveColor.B = SetupJsonObject->GetNumberField(TEXT("EmissiveColorB"));
	EmissiveColor.A = SetupJsonObject->GetNumberField(TEXT("EmissiveColorA"));

	Metallic = SetupJsonObject->GetNumberField(TEXT("Metallic"));
	Roughness = SetupJsonObject->GetNumberField(TEXT("Roughness"));

	if (Mat)
	{
		Mat->SetVectorParameterValue("Color", MeshColor);
		Mat->SetVectorParameterValue("EmissiveColor", EmissiveColor);
		Mat->SetScalarParameterValue("Metallic", Metallic);
		Mat->SetScalarParameterValue("Roughness", Roughness);
	}
}

void ASW_CreatorStaticObject::OnRep_MeshColorChanged()
{
	if (Mat)
	{
		Mat->SetVectorParameterValue("Color", MeshColor);
	}
}

void ASW_CreatorStaticObject::OnRep_EmissiveColorChanged()
{
	if (Mat)
	{
		Mat->SetVectorParameterValue("EmissiveColor", EmissiveColor);
	}
}

void ASW_CreatorStaticObject::OnRep_MeshMetallicChanged()
{
	if (Mat)
	{
		Mat->SetScalarParameterValue("Metallic", Metallic);
	}
}

void ASW_CreatorStaticObject::OnRep_MeshRoughnessChanged()
{
	if (Mat)
	{
		Mat->SetScalarParameterValue("Roughness", Roughness);
	}
}

void ASW_CreatorStaticObject::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASW_CreatorStaticObject, MeshColor);
	DOREPLIFETIME(ASW_CreatorStaticObject, EmissiveColor);
	DOREPLIFETIME(ASW_CreatorStaticObject, Metallic);
	DOREPLIFETIME(ASW_CreatorStaticObject, Roughness);
}
