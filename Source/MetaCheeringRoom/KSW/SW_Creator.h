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
	GizmoDrag,
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

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Del;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Alt;

	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Ctrl;

	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_C;

	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_V;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_CameraSpeedUp;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_CameraSpeedDown;

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
	
	UFUNCTION()
	void OnMyDelete(const FInputActionValue& Value);
	
	UFUNCTION()
	void OnMyCtrlStarted(const FInputActionValue& Value);
	
	UFUNCTION()
	void OnMyCtrlCompleted(const FInputActionValue& Value);
	
	UFUNCTION()
	void OnMyAltStarted(const FInputActionValue& Value);
	
	UFUNCTION()
	void OnMyAltCompleted(const FInputActionValue& Value);
	
	UFUNCTION()
	void OnMyC(const FInputActionValue& Value);
	
	UFUNCTION()
	void OnMyV(const FInputActionValue& Value);

	UFUNCTION()
	void OnMyCameraSpeedUp(const FInputActionValue& Value);
	UFUNCTION()
	void OnMyCameraSpeedDown(const FInputActionValue& Value);
	
	FVector Direction;

	// 서버에서 동기화할 회전 값
	UPROPERTY(ReplicatedUsing = OnRep_CurrentRotation)
	FRotator CurrentRotation; 

	ECreatorMouseState MouseState;

	UPROPERTY()
	class ASW_CreatorPlayerController* PC;

	void SetMouseState(ECreatorMouseState NewState);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class USW_CameraSpeedWidget> CameraSpeedWidgetFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class USW_CameraSpeedWidget* CameraSpeedWidget;

	// 마우스 클릭시 초기 위치
	FVector2D MouseDownPosition;

	bool bIsCtrlPressed = false;
	bool bIsAltPressed = false;

	float LastInputTime = 0.0f;
	float InputDelay = 0.2f;

	float CameraSpeed = 500.0f;
	UFUNCTION(Server, Unreliable)
	void Server_Movement(const FVector& NewLocation);
	UFUNCTION(Server, Unreliable)
	void Server_Rotation(const FRotator& NewRotation);

	UFUNCTION()
	void OnRep_CurrentRotation();

	// 플레이어컨트롤에서 해줘도 되지만 UI 업데이트가 안되는 관계로 Player에서 처리
	// 멀티 케스트
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AddCreatingDummyObject(class ASW_CreatorObject* NewCreatingObject, const struct FCreatorObjectData& ObjectData);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DeleteObjectInfo(class ASW_CreatorObject* DeleteObject);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DeleteObject(class ASW_CreatorObject* DeleteObject);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DetachObject(class ASW_CreatorObject* DetachObject);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AttachObject(class ASW_CreatorObject* ParentObject, class ASW_CreatorObject* AttachObject);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CopyPasteObject(class ASW_CreatorObject* CopyObject);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// SceneComponent/
	UPROPERTY(EditAnywhere)
	class USceneComponent* RootSceneComponent;

	// 스켈레탈 메쉬
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* HandMeshComponent;

	// 카메라
	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComponent;

};
