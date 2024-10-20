// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SW_Creator.generated.h"

// Mode State
UENUM(BlueprintType)
enum class ECreatorMouseState : uint8
{
	None,
	Clicked,
	Drag,
};

UCLASS()
class METACHEERINGROOM_API ASW_Creator : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASW_Creator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputMappingContext* IMC_Player;
	
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_Move;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Look;

	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Q;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_W;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_E;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_R;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_RClick;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_LClick;

	UFUNCTION()
	void OnMyMove(const FInputActionValue& Value);
	UFUNCTION()
	void OnMyLook(const FInputActionValue& Value);

	UFUNCTION()
	void OnMyQ(const FInputActionValue& Value);
	UFUNCTION()
	void OnMyW(const FInputActionValue& Value);
	UFUNCTION()
	void OnMyE(const FInputActionValue& Value);
	UFUNCTION()
	void OnMyR(const FInputActionValue& Value);
	UFUNCTION()
	void OnMyRClick(const FInputActionValue& Value);
	UFUNCTION()
	void OnMyLClick(const FInputActionValue& Value);

	UFUNCTION()
	void OnMyRClickStarted(const FInputActionValue& Value);
	UFUNCTION()
	void OnMyRClickCompleted(const FInputActionValue& Value);

	UFUNCTION()
	void OnMyLClickStarted(const FInputActionValue& Value);
	UFUNCTION()
	void OnMyLClickCompleted(const FInputActionValue& Value);
	
	FVector Direction;

	ECreatorMouseState MouseState;

	UPROPERTY()
	class ASW_CreatorPlayerController* PC;

	void SetMouseState(ECreatorMouseState NewState);

};
