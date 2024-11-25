// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HG_ItemBase.h"
#include "HG_Player.generated.h"

class AHG_EquipItem;
class UMaterialInstanceDynamic;

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

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Custom;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Teleport1;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Teleport2;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Teleport3;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_FullScreen;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Shake;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Sit;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_CheerSurfing;

	UFUNCTION()
	void CheerSurfing();
	UFUNCTION()
	void Sit();

	UFUNCTION()
	void ShakeHand();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AHG_CheeringStick> CSClass;

	AHG_CheeringStick* My_CheeringStick;

	TArray<AActor*> CheerSticks;

	UFUNCTION()
	void StopHand();

	UFUNCTION(Server,Reliable)
	void ServerRPC_Shake(bool Value);

	UFUNCTION()
	void ConversionFullScreen();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> FullScreenClass;
	
	UPROPERTY()
	UUserWidget* FullScreenWidget;

	UFUNCTION()
	void TeleportToStore();

	UFUNCTION()
	void TeleportToJoin();

	UFUNCTION()
	void TeleportToCreate();


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
	void MulticastRPCEquipItemToSocket(AHG_EquipItem* Value);

	UFUNCTION(Server, Reliable)
	void ServerRPCUnequipItemToSocket(const FString& NameValue);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCUnequipItemToSocket(const FString& NameValue);

	void DestroyItem(AHG_ItemBase* ItemValue);

	void SpawnedMulticast();
	UFUNCTION(Server, Reliable)
	void ServerRPCDestroyItem(AHG_ItemBase* ItemValue);

	void SpawnItem(FItemData p_ItemInfo);

	UFUNCTION(Server, Reliable)
	void ServerRPCSpawnItem(FItemData p_ItemInfo);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCSpawnItem(AHG_ItemBase* ItemValue);

	UPROPERTY(Replicated)
	bool bEquipItem = false;

	UPROPERTY(Replicated)
	TArray<AHG_EquipItem*> EquipItemList;

	TArray<FItemData> InitEquipItemList;
	bool bDetectStand = false;

	float TargetValue1 = 150.0f;
	float TargetValue2 = -50.0f;

	FVector LookingPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
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
	UFUNCTION(Server, Reliable)
	void ServerRPC_Emotion(UAnimMontage* Montage);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Emotion(UAnimMontage* Montage);

	UFUNCTION(Server, Reliable)
	void ServerRPC_InitCharacter(AHG_Player* Value);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_InitCharacter(AHG_Player* Value);

	float Timing = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UHG_RemoteCS> RCSClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class APlayerController* PC;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UHG_HUD> HeadUpClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UHG_HUD* HUD;
	
	UPROPERTY()
	class AHG_EquipItem* EItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Gender = 1;

	UPROPERTY(EditDefaultsOnly, Replicated)
	class USkeletalMesh* SkeletalMesh;

	UFUNCTION(BlueprintCallable)
	void PopUpHUD();

	UFUNCTION()
	void ApplyCustomizing(FLinearColor Cloth, UTexture2D* ClothTexture, FLinearColor ClothHem, FLinearColor Eyes, FLinearColor Hair, FLinearColor HairPin);

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial_Cloth;
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial_HairPin;
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial_ClothColor;
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial_Eyes;
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial_Hair;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UHG_CustomUI> CustomUIClass;

	void PopUpCustomUI();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ApplyCustom(FLinearColor Cloth, UTexture2D* ClothTexture, FLinearColor ClothHem, FLinearColor Eyes, FLinearColor Hair, FLinearColor HairPin);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ApplyCustom(FLinearColor Cloth, UTexture2D* ClothTexture, FLinearColor ClothHem, FLinearColor Eyes, FLinearColor Hair, FLinearColor HairPin);

	UFUNCTION()
	void UpdateClothColor();

	UFUNCTION()
	void UpdateHairColor();

	UFUNCTION()
	void UpdateHairPinColor();

	UFUNCTION()
	void UpdateEyesColor();

	UFUNCTION()
	void UpdateClothHemColor();

	UFUNCTION()
	void UpdateClothPrinting();
	UPROPERTY(ReplicatedUsing = OnRep_ClothColor)
	FLinearColor CurCloth = FLinearColor(1.0f, 1.0f, 1.0f);

	UFUNCTION()
	void OnRep_ClothColor();

	UPROPERTY(ReplicatedUsing = OnRep_ClothHemColor)
	FLinearColor CurClothHem = FLinearColor(1.0f, 1.0f, 1.0f);

	UFUNCTION()
	void OnRep_ClothHemColor();

	UPROPERTY(ReplicatedUsing = OnRep_HairPinColor)
	FLinearColor CurHairPin = FLinearColor(1.0f, 1.0f, 1.0f);

	UFUNCTION()
	void OnRep_HairPinColor();

	UPROPERTY(ReplicatedUsing = OnRep_HairColor)
	FLinearColor CurHair = FLinearColor(1.0f, 1.0f, 1.0f);

	UFUNCTION()
	void OnRep_HairColor();

	UPROPERTY(ReplicatedUsing = OnRep_EyesColor)
	FLinearColor CurEyes = FLinearColor(1.0f, 1.0f, 1.0f);

	UFUNCTION()
	void OnRep_EyesColor();

	UPROPERTY(ReplicatedUsing = OnRep_Printing)
	UTexture2D* CurPrinting = nullptr;

	UFUNCTION()
	void OnRep_Printing();

	void RemoveInventory();

	UPROPERTY(EditDefaultsOnly)
	class USoundWave* PurchaseSound;

	UPROPERTY(EditDefaultsOnly)
	class USoundWave* GetPointSound;

	UPROPERTY(EditDefaultsOnly)
	class USoundWave* TeleportSound;

	UPROPERTY(EditDefaultsOnly)
	class USoundWave* UIPopUpSound;
	UPROPERTY(EditDefaultsOnly)
	class USoundWave* UIPopUpSound2;

	void StartFeverTime();

	UFUNCTION(Server, Reliable)
	void ServerRPC_StartFeverTime();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StartFeverTime();

	void EndFeverTime();

	UFUNCTION(Server, Reliable)
	void ServerRPC_EndFeverTime();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_EndFeverTime();

	void BlingBling();

	bool bFeverTime = false;

	bool bToggle3 = false;
	float Intensity = 1.0f;


	FTimerHandle FeverTimeHandle;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AHG_KomanoDummy> KomanoDummyClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated)
	bool bIsShaking = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Replicated)
	bool bIsSitting = false;

	UFUNCTION(Server,Reliable)
	void ServerRPC_SetSitState();	


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class AHG_ChairCollision* DetectChair;

	bool bSitToggle = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Replicated)
	bool bIsCheerSurfing = false;

	UFUNCTION(Server,Reliable)
	void ServerRPC_SetCheerSurfingState();	

	void SetCheerSurfingState();

	void CameraDelay();

	bool bOnFullScreen = false;
	bool bOnInventory = false;

	bool bGetCoinSound = false;
};
