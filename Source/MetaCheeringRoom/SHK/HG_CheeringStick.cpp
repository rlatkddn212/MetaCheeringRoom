// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_CheeringStick.h"
#include "SHK/HG_HUD.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"

AHG_CheeringStick::AHG_CheeringStick()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHG_CheeringStick::BeginPlay()
{
	Super::BeginPlay();
	this->ItemData.ItemName = Name;
	InitItemData();

	UMaterialInterface* Mat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/SHK/Material/M_light_Inst.M_light_Inst'"));
	if (Mat)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(Mat, this);
		if (DynamicMaterial)
		{
			MeshComp->SetMaterial(0, DynamicMaterial);
			MeshComp->SetMaterial(1, DynamicMaterial);
		}
	}
}

void AHG_CheeringStick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bBling)
	{
		BlingBling(DeltaTime);
	}
}

void AHG_CheeringStick::Equiped(APawn* Value)
{
	SetOwner(Value);

	if (Owner != nullptr && Owner->IsLocallyControlled())
	{
		HUD = CreateWidget<UHG_HUD>(GetWorld(), HeadUpClass);
		if (HUD)
		{
			HUD->AddToViewport();
		}
	}
}

void AHG_CheeringStick::ChangeColor(FLinearColor Value)
{
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("Color", Value);
	}
}

void AHG_CheeringStick::ChangeIntensity(float Value)
{
	if (DynamicMaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f"), Value);
		DynamicMaterial->SetScalarParameterValue("Intensity", Value);
		CurrentIntensity = Value;
	}
}

void AHG_CheeringStick::ApplyChange(FLinearColor Color, bool Bling, float Intensity)
{
	ChangeIntensity(Intensity);
	ChangeColor(Color);
	bBling = Bling;
}

void AHG_CheeringStick::BlingBling(float DeltaSecond)
{
	if (!bToggle)
	{
		CurrentIntensity -= DeltaSecond*100;
		ChangeIntensity(CurrentIntensity);
		UE_LOG(LogTemp, Warning, TEXT("down"));
		if (CurrentIntensity < MinIntensity)
		{
			bToggle = true;
		}
	}
	else
	{
		CurrentIntensity += DeltaSecond*100;
		ChangeIntensity(CurrentIntensity);
		UE_LOG(LogTemp, Warning, TEXT("Up"));
		if (CurrentIntensity > MaxIntensity)
		{
			bToggle = false;
		}
	}
}
