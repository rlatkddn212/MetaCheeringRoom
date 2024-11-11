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

	virtual void OnChangeProperty(int32 id, UCreatorPropertyBase* CreatorProperty);
	virtual UCreatorPropertyBase* GetProperty(int32 id);

	virtual void RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const override;
	virtual void SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject) override;

	// PointLight
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPointLightComponent* LightComp;
	FLinearColor LightColor;
};
