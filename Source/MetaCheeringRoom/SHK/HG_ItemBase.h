// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HG_ItemBase.generated.h"


UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	Category_OneHandGrab UMETA(DisplayName = "OneHandGrab"),
	Category_TwoHandGrab UMETA(DisplayName = "TwoHandGrab"),
	Category_Top UMETA(DisplayName = "Top"),
	Category_Bottom UMETA(DisplayName = "Bottom"),
	Category_Active UMETA(DisplayName = "Active"),
	Category_Emotion UMETA(DisplayName = "Emotion"),
	Category_Emoji UMETA(DisplayName = "Emoji"),
	Category_Sound UMETA(DisplayName = "Sound"),
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemPrice;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHG_ItemBase> ItemClass;

	UPROPERTY(EditDefaultsOnly)
	EItemCategory ItemCategory;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* Woman_Montage; 
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* Man_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundWave> Sound;

	bool operator==(const FItemData& Other) const
	{
		return ItemName == Other.ItemName;
	}

	FItemData() : ItemName(TEXT("")), ItemIcon(nullptr), ItemPrice(0),ItemClass(nullptr), Woman_Montage(nullptr), Man_Montage(nullptr){}

	FItemData(const FString& Name): ItemName(Name), ItemIcon(nullptr), ItemPrice(0),ItemClass(nullptr), Woman_Montage(nullptr), Man_Montage(nullptr){}
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

	FItemData ItemData;

private:

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;

	virtual void Use();

	virtual void SetItemOwner(APawn* Value);

	void SetItemName(FString Value);
	FString GetItemName();

	void SetItemIcon(UTexture2D* Value);
	UTexture2D* GetItemIcon();

	FItemData GetItemData();
	void SetItemData(FItemData ItemValue);

	EItemCategory GetItemCategory();
	void SetItemCategory(EItemCategory p_Category);

	virtual void InitItemData();

	UPROPERTY()
	class UHG_GameInstance* GI;

	UPROPERTY()
	class APawn* ItemOwner;

	bool bEquiped = false;
};
