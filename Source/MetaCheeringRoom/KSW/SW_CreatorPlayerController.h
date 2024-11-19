// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SW_CreatorPlayerController.generated.h"

// 상태
UENUM(BlueprintType)
enum class ECreatorToolState : uint8
{
	Selection,
	Position,
	Rotation,
	Scale
};

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API ASW_CreatorPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASW_CreatorPlayerController();
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// 마우스 오버됨
	void OnMouseOver();

	// 마우스 왼쪽 클릭 처리 함수
	bool OnLeftClick();

	void CreatingDummyObject(struct FCreatorObjectData* ObjectData);
	// 지워지지 않는경우

	UFUNCTION(Server, Reliable)
	void Server_CreatingDummyObject(const struct FCreatorObjectData& ObjectData);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CreatingDummyObject(class ASW_CreatorObject* NewDummyObject);

	UFUNCTION(Server, Reliable)
	void Server_SetOwnerObject(class ASW_CreatorObject* OwnerObject, bool isOnwer);

	UFUNCTION(Server, Reliable)
	void Server_DeleteCreatingDummyObject(class ASW_CreatorObject* DummyObject);

	UFUNCTION(Server, Reliable)
	void Server_AddCreatingDummyObject(class ASW_CreatorObject* NewCreatingObject);

	UFUNCTION(Server, Reliable)
	void Server_DeleteObject(class ASW_CreatorObject* DeleteObject);

	UFUNCTION()
	void DoSelectObject(class ASW_CreatorObject* NewSelectObject);
	
	// 서버에서 삭제된 오브젝트가 선택되어있는경우 선택해제
	void UnSelectServerDeleteObject(class ASW_CreatorObject* ServerDeleteObject);

	UFUNCTION()
	bool EndDragDummyObject();
	void MoveDummyObject(FVector2D MousePosition);

	void SetToolState(ECreatorToolState NewState);
	ECreatorToolState GetToolState() { return ToolState; }

	// 상태
	UPROPERTY()
	ECreatorToolState ToolState;

	class ASW_CreatorObject* SelectedObject;
	
	UPROPERTY()
	class ASW_CreatorObject* CreatingObject;
	
	// 위젯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class USW_CreatorWidget> CreatorWidgetFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class USW_CreatorWidget* CreatorWidget;

	void Drag(FVector2D MouseDownPosition);
	void DragEnd();

	void OnObjectChanged();
	void DeleteSelectedObject();

	void ReloadHierarchy();

	void CopySelectedObject();
	void PasteSelectedObject();

	class ASW_CreatorObject* CopiedObject;

};
