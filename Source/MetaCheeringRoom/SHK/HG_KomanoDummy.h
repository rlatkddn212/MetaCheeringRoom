// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../KSW/CreatorObject/SW_CreatorSkeletonObject.h"
#include "HG_KomanoDummy.generated.h"

UCLASS()
class METACHEERINGROOM_API AHG_KomanoDummy : public ASW_CreatorSkeletonObject
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHG_KomanoDummy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server,Reliable)
	void ServerRPC_SetStateSit(bool Value);	
	UFUNCTION(Server,Reliable)
	void ServerRPC_SetStateIdle(bool Value);
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetStateShake(bool Value);

	UFUNCTION(NetMulticast,Reliable)
	void Multicast_SetStateSit(bool Value);
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_SetStateIdle(bool Value);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetStateShake(bool Value);


	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bSit = true;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bShake = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIdle = false;

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeCheeringStickColor();	
	
	UFUNCTION(BlueprintImplementableEvent)
	void ChangeCSOrigin();

	void ReturnOriginCondition();

	void CheerSurfing();

};
