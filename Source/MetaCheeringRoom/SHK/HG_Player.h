// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HG_ItemBase.h"
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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UHG_ItemPurchaseWidget> PurchaseWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	UHG_ItemPurchaseWidget* PurchaseWidget;

	void PopUpPurchaseWidget();

	FItemData TempData;

	UPROPERTY(EditDefaultsOnly)
	AActor* LookAtActor;

	bool bIsStand = false;

	UPROPERTY(EditDefaultsOnly)
	class AHG_DisplayStandBase* DetectedStand;

	// ========================������ ���===============================

	UPROPERTY(EditDefaultsOnly , Category = Equip)
	class USceneComponent* HandComp;
	UPROPERTY(EditDefaultsOnly , Category = Equip)
	class USceneComponent* LowerComp;
	UPROPERTY(EditDefaultsOnly, Category = Equip)
	class USceneComponent* UpperComp;

	void EquipItem(class AHG_EquipItem* ItemValue);

	void UnequipItem(const FString& NameValue);
	UPROPERTY()
	AHG_EquipItem* GrabItem;

	UPROPERTY(EditDefaultsOnly , Category = Equip)
	float GrabDistance = 300;

	void EquipItemToSocket(AHG_EquipItem* ItemValue);
	void UnequipItemToSocket(const FString& NameValue);

	UFUNCTION(Server,Reliable)
	void ServerRPCEquipItemToSocket(AHG_EquipItem* ItemValue);
	
	UFUNCTION(NetMulticast,Reliable)
	void MulticastRPCEquipItemToSocket(AHG_EquipItem* ItemValue);
	
	UFUNCTION(Server, Reliable)
	void ServerRPCUnequipItemToSocket(const FString& NameValue);

	UFUNCTION(NetMulticast,Reliable)
	void MulticastRPCUnequipItemToSocket(const FString& NameValue);

	bool bEquipItem = false;

	TArray<AHG_EquipItem*> EquipItemList;

	UPROPERTY(EditDefaultsOnly)
	class UTimelineComponent* Timeline;

	bool bDetectStand = false;

	float TargetValue1 = 200.0f;
	float TargetValue2 = -50.0f;

};
