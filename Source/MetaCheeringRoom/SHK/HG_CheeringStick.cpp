// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_CheeringStick.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "HG_Player.h"
#include "Net/UnrealNetwork.h"

AHG_CheeringStick::AHG_CheeringStick()
{
	PrimaryActorTick.bCanEverTick = true;

	this->bReplicates = true;
	SetReplicates(true);
	SetActorHiddenInGame(true);
}

void AHG_CheeringStick::BeginPlay()
{
	Super::BeginPlay();
	this->ItemData.ItemName = Name;
	InitItemData();

	this->SetActorHiddenInGame(true);


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

	time -= DeltaTime;

	if (time < 0)
	{
		this->SetActorHiddenInGame(false);
	}
	if (bBling)
	{
		BlingBling(DeltaTime);
	}
}

void AHG_CheeringStick::Equiped(APawn* Value)
{
	SetItemOwner(Value);
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
		DynamicMaterial->SetScalarParameterValue("Intensity", Value);
		CurrentIntensity = Value;
	}
}

void AHG_CheeringStick::ApplyChange(FLinearColor Color, bool Bling, float Intensity)
{
	ServerRPC_ApplyChange(Color, Bling, Intensity);
}

void AHG_CheeringStick::ServerRPC_ApplyChange_Implementation(FLinearColor Color, bool Bling, float Intensity)
{
	Multicast_ApplyChange(Color, Bling, Intensity);
}

void AHG_CheeringStick::Multicast_ApplyChange_Implementation(FLinearColor Color, bool Bling, float Intensity)
{
	ChangeIntensity(Intensity);
	ChangeColor(Color);
	bBling = Bling;
}

void AHG_CheeringStick::BlingBling(float DeltaSecond)
{
	if (!bToggle)
	{
		CurrentIntensity -= DeltaSecond * 100;
		ChangeIntensity(CurrentIntensity);
		if (CurrentIntensity < MinIntensity)
		{
			bToggle = true;
		}
	}
	else
	{
		CurrentIntensity += DeltaSecond * 100;
		ChangeIntensity(CurrentIntensity);
		if (CurrentIntensity > MaxIntensity)
		{
			bToggle = false;
		}
	}
}

void AHG_CheeringStick::OnRep_ChangeMatColor()
{
	ChangeColor(MatColor);
}

void AHG_CheeringStick::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHG_CheeringStick,MatColor);
}

