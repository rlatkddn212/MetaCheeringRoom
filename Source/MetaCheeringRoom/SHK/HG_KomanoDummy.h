// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HG_KomanoDummy.generated.h"

UCLASS()
class METACHEERINGROOM_API AHG_KomanoDummy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHG_KomanoDummy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly)
	class UAnimSequence* Sit;

	UFUNCTION(Server,Reliable)
	void ServerRPC_SetStateSit(bool Value);	
	UFUNCTION(Server,Reliable)
	void ServerRPC_SetStateIdle(bool Value);
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetStateShake(bool Value);


	UPROPERTY(EditAnywhere,BlueprintReadWrite,Replicated)
	bool bSit = true;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Replicated)
	bool bShake = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Replicated)
	bool bIdle = false;

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeCheeringStickColor();
};
