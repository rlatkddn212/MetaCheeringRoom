// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SHK/HG_ItemBase.h"
#include "HG_EmojiItemBase.generated.h"

/**
 *
 */
UCLASS()
class METACHEERINGROOM_API AHG_EmojiItemBase : public AHG_ItemBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
	virtual void Use() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FString Name;

	void ChangeMeshMaterial();

	void ChangeMaterialTexture();
};
