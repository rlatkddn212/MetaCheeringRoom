// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KSW/CreatorObject/SW_CreatorObject.h"
#include "SW_CreatorStaticObject.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API ASW_CreatorStaticObject : public ASW_CreatorObject
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ASW_CreatorStaticObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnSelected(bool isSelected) override;

	virtual void OnChangeProperty(int32 id, UCreatorPropertyBase* CreatorProperty);
	virtual TMap<int32, UCreatorPropertyBase*> GetPropertyMap() override;

	virtual void RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const override;
	virtual void SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Mesh;
	
	UPROPERTY(ReplicatedUsing = OnRep_MeshColorChanged)
	FLinearColor MeshColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

	UPROPERTY(ReplicatedUsing = OnRep_EmissiveColorChanged)
	FLinearColor EmissiveColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	
	UPROPERTY(ReplicatedUsing = OnRep_MeshMetallicChanged)
	float Metallic = 0.0f;
	UPROPERTY(ReplicatedUsing = OnRep_MeshRoughnessChanged)
	float Roughness = 0.0f;

	// ---------------------------------------------------------- server 동기화 부분
	UFUNCTION()
	void OnRep_MeshColorChanged();

	UFUNCTION()
	void OnRep_EmissiveColorChanged();

	UFUNCTION()
	void OnRep_MeshMetallicChanged();

	UFUNCTION()
	void OnRep_MeshRoughnessChanged();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
