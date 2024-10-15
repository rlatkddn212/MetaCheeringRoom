// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HG_Player.generated.h"

UCLASS()
class METACHEERINGROOM_API AHG_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHG_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_Player;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Jump;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Look;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Interaction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Inventory;

	UFUNCTION()
	void OnMyMove(const FInputActionValue& Value);
	UFUNCTION()
	void OnMyJump(const FInputActionValue& Value);
	UFUNCTION()
	void OnMyLook(const FInputActionValue& Value);
	UFUNCTION()
	void OnMyInteraction(const FInputActionValue& Value);
	UFUNCTION()
	void DetectObject();
	UFUNCTION()
	void PopUpInventory(const FInputActionValue& Value);
	FVector Direction;

	UPROPERTY(EditDefaultsOnly)
	class UHG_PlayerGoodsComponent* GoodsComp;

	bool bToggle = false;
	bool bToggle2 = false;
	bool bCanMove = true;

	// ============== Inventory ====================
	UPROPERTY(EditDefaultsOnly)
	class UHG_PlayerInventoryComponent* InventoryComp;

	UPROPERTY(EditDefaultsOnly)
	class UInventoryWidget* InventoryWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInventoryWidget>InventoryWidgetClass;
};
