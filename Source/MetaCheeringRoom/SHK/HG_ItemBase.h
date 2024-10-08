// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HG_ItemBase.generated.h"


USTRUCT(BlueprintType)
struct FItemData: public FTableRowBase
{
    GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemPrice;
};

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
	FItemData ItemData;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;

	// 아이템 사용 (자식에서 구현)
	virtual void Use();

	void SetItemName(FString Value);
	FString GetItemName();

	void SetItemIcon(UTexture2D* Value);
	UTexture2D* GetItemIcon();
};
