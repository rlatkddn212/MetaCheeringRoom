// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KSW/CreatorObject/SW_CreatorObject.h"
#include "SW_CreatorSkyAtomsphere.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API ASW_CreatorSkyAtomsphere : public ASW_CreatorObject
{
	GENERATED_BODY()
public:
	ASW_CreatorSkyAtomsphere();
	virtual void BeginPlay() override;

	virtual void OnChangeProperty(int32 id, UCreatorPropertyBase* CreatorProperty);
	virtual TMap<int32, UCreatorPropertyBase*> GetPropertyMap() override;

	virtual void RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const override;
	virtual void SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject) override;

};
