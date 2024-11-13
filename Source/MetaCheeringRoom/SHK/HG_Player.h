// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HG_ItemBase.h"
#include "HG_Player.generated.h"

class AHG_EquipItem;

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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Emotion;

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
	UFUNCTION()
	void Emotion();


	FVector Direction;

	UPROPERTY(EditDefaultsOnly)
	class UHG_PlayerGoodsComponent* GoodsComp;

	bool bToggle = false;
	bool bToggle2 = false;
	bool bStoreWidget = false;
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

	UPROPERTY(EditDefaultsOnly, Category = Equip)
	class USceneComponent* HandLComp;
	UPROPERTY(EditDefaultsOnly, Category = Equip)
	class USceneComponent* HandRComp;
	UPROPERTY(EditDefaultsOnly, Category = Equip)
	class USceneComponent* LowerComp;
	UPROPERTY(EditDefaultsOnly, Category = Equip)
	class USceneComponent* UpperComp;

	class UHG_GameInstance* GI;

	void EquipItem(AHG_EquipItem* ItemValue);

	void UnequipItem(const FString& NameValue);

	UPROPERTY(EditDefaultsOnly, Category = Equip)
	float GrabDistance = 300;

	void EquipItemToSocket(FItemData p_ItemInfo);

	void UnequipItemToSocket(const FString& NameValue);

	UFUNCTION(Server, Reliable)
	void ServerRPCEquipItemToSocket(FItemData p_ItemInfo);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCEquipItemToSocket(AHG_EquipItem* ItemValue);

	UFUNCTION(Server, Reliable)
	void ServerRPCUnequipItemToSocket(const FString& NameValue);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCUnequipItemToSocket(const FString& NameValue);

	void DestroyItem(AHG_ItemBase* ItemValue);

	UFUNCTION(Server, Reliable)
	void ServerRPCDestroyItem(AHG_ItemBase* ItemValue);

	void SpawnItem(FItemData p_ItemInfo);

	UFUNCTION(Server, Reliable)
	void ServerRPCSpawnItem(FItemData p_ItemInfo);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCSpawnItem(AHG_ItemBase* ItemValue);

	bool bEquipItem = false;

	UPROPERTY(Replicated)
	TArray<AHG_EquipItem*> EquipItemList;

	TArray<FItemData> InitEquipItemList;
	bool bDetectStand = false;

	float TargetValue1 = 150.0f;
	float TargetValue2 = -50.0f;

	FVector LookingPoint;

	class UHG_PlayerAnimInstance* Anim;

	bool bPassed = false;

	// 상점 안 UI
	UPROPERTY()
	class UHG_StoreWidget* StoreWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UHG_StoreWidget> StoreWidgetFactory;

	UFUNCTION()
	void CreateStoreWidget();

	UFUNCTION()
	void EnterTheStore();
	UFUNCTION()
	void ExitTheStore();

	// 오디오
	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* AudioComp;

// 	UPROPERTY(ReplicatedUsing = OnRep_PlayEmotionMontage)
// 	bool bPlayEmotionMontage;

// 	void StartEmotion(UAnimMontage* Montage);
// 
// 	UFUNCTION()
// 	void OnRep_PlayEmotionMontage();

// 	UPROPERTY(Replicated)
// 	UAnimMontage* SelectedMontage;
	UFUNCTION(Server,Reliable)
	void ServerRPC_Emotion(UAnimMontage* Montage);
	UFUNCTION(NetMulticast,Reliable)
	void MulticastRPC_Emotion(UAnimMontage* Montage);

	class UObjectPool* ObjectPool;

	float Timing = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UHG_RemoteCS> RCSClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	class APlayerController* PC;
};
