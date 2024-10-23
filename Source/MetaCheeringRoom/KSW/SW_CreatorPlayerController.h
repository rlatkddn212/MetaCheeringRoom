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

	// 마우스 왼쪽 클릭 처리 함수
	bool OnLeftClick();

	void CreatingDummyObject(struct FCreatorObjectData* ObjectData);
	// 지워지지 않는경우

	UFUNCTION()
	void DoSelectObject(class ASW_CreatorObject* NewSelectObject);
	
	UFUNCTION()
	bool DeleteDummyObject();
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
};
