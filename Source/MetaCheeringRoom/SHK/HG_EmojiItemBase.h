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

public:
	AHG_EmojiItemBase();
protected:
	virtual void BeginPlay() override;
public:
	virtual void Use() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString Name;

	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeMeshMaterial();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_ChangeMeshMaterial();

	void ChangeMeshMaterial();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeMaterialTexture();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_ChangeMaterialTexture();

	void ChangeMaterialTexture();

	UFUNCTION(Server,Reliable)
	void ServerRPC_FollowPlayer();

	UFUNCTION(NetMulticast,Reliable)
	void MulticastRPC_FollowPlayer();

	void FollowPlayer();

	UPROPERTY(Replicated)
	FRotator Rot;
	
	UPROPERTY(Replicated)
	FVector Loc;

	float SpawnTime;
};
