// Fill out your copyright notice in the Description page of Project Settings.


#include "SW_CreatorStaticObject.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "KSW/CreatorGizmo/CreatorPositionGizmoComponent.h"
#include "KSW/CreatorGizmo/CreatorRotationGizmoComponent.h"
#include "KSW/CreatorGizmo/CreatorScaleGizmoComponent.h"

ASW_CreatorStaticObject::ASW_CreatorStaticObject()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

}

void ASW_CreatorStaticObject::BeginPlay()
{
	Super::BeginPlay();
	// 매쉬의 머터리얼을 가져온다.
	Mat = UMaterialInstanceDynamic::Create(Mesh->GetMaterial(0), this);
	Mesh->SetMaterial(0, Mat);
	MeshColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
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

void ASW_CreatorStaticObject::OnChangeColor(FLinearColor Color)
{
	Super::OnChangeColor(Color);

	MeshColor = Color;
	if (Mat)
	{
		Mat->SetVectorParameterValue("Color", Color);
	}
}

FLinearColor ASW_CreatorStaticObject::GetColor()
{
	return MeshColor;
}

void ASW_CreatorStaticObject::RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const
{
	Super::RecordJsonAdditionalInfo(RecordJsonObject);
	RecordJsonObject->SetNumberField(TEXT("ColorR"), MeshColor.R);
	RecordJsonObject->SetNumberField(TEXT("ColorG"), MeshColor.G);
	RecordJsonObject->SetNumberField(TEXT("ColorB"), MeshColor.B);
	RecordJsonObject->SetNumberField(TEXT("ColorA"), MeshColor.A);
}

void ASW_CreatorStaticObject::SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject)
{
	Super::SetupJsonAdditionalInfo(SetupJsonObject);
	// 추가 정보를 설정한다.
	MeshColor.R = SetupJsonObject->GetNumberField(TEXT("ColorR"));
	MeshColor.G = SetupJsonObject->GetNumberField(TEXT("ColorG"));
	MeshColor.B = SetupJsonObject->GetNumberField(TEXT("ColorB"));
	MeshColor.A = SetupJsonObject->GetNumberField(TEXT("ColorA"));

	OnChangeColor(MeshColor);
}
