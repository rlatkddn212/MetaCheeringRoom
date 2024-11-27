// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KSW/CreatorObject/SW_CreatorLight.h"
#include "SW_CreatorPointLight.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API ASW_CreatorPointLight : public ASW_CreatorLight
{
	GENERATED_BODY()

public:
	ASW_CreatorPointLight();

	virtual void BeginPlay() override;

	virtual void CopyCreatorObject(ASW_CreatorObject* CopyObject) override;

	virtual void OnChangeProperty(int32 id, UCreatorPropertyBase* CreatorProperty);
	virtual TMap<int32, UCreatorPropertyBase*> GetPropertyMap() override;

	virtual void RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const override;
	virtual void SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject) override;

	// PointLight
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPointLightComponent* LightComp;

	UPROPERTY(ReplicatedUsing = OnRep_LightColorChanged)
	FLinearColor LightColor;

	UPROPERTY(ReplicatedUsing = OnRep_LightIntensityChanged)
	float LightIntensity = 5000.0f;

	UPROPERTY(ReplicatedUsing = OnRep_LightAttenuationRadiusChanged)
	float LightAttenuationRadius = 1000.0f;

	UPROPERTY(ReplicatedUsing = OnRep_LightSourceRadiusChanged)
	float LightSourceRadius = 0.0f;

	float LightSourceLength;

	UPROPERTY(ReplicatedUsing = OnRep_LightCastShadowChanged)
	bool CastShadow = true;

	UFUNCTION()
	void OnRep_LightColorChanged();

	UFUNCTION()
	void OnRep_LightIntensityChanged();

	UFUNCTION()
	void OnRep_LightAttenuationRadiusChanged();

	UFUNCTION()
	void OnRep_LightSourceRadiusChanged();

	UFUNCTION()
	void OnRep_LightCastShadowChanged();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
