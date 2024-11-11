// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KSW/CreatorObject/SW_CreatorLight.h"
#include "../CreatorMapSubsystem.h"
#include "SW_CreatorDirectionalLight.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API ASW_CreatorDirectionalLight : public ASW_CreatorLight
{
	GENERATED_BODY()
	
public:
	ASW_CreatorDirectionalLight();

	virtual void BeginPlay() override;

	virtual void OnChangeProperty(int32 id, UCreatorPropertyBase* CreatorProperty);
	virtual UCreatorPropertyBase* GetProperty(int32 id);

	virtual void RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const override;
	virtual void SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject) override;

	// DirectionalLight
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDirectionalLightComponent* LightComp;
	FLinearColor LightColor;
};
