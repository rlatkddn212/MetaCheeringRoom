// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HG_ItemBase.generated.h"

UCLASS()
class METACHEERINGROOM_API AHG_ItemBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHG_ItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(Replicated)
	FString ItemName;

	UPROPERTY(Replicated)
	UTexture2D* ItemIcon;

	UPROPERTY(Replicate)
	int32 Qunatity;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;

	// 아이템 사용 (자식에서 구현)
	void UseItem();

	void SetItemName(FString Value);
	FString GetItemName() const;

	void SetItemIcon(UTexture2D* Value);
	UTexture2D* GetItemIcon() const;

	void SetQunatity(int32 Value);
	int32 GetQuantity() const;

};
