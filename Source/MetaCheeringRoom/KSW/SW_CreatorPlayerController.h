// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SW_CreatorPlayerController.generated.h"

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

	// ���콺 ���� Ŭ�� ó�� �Լ�
	void OnLeftClick();

	void CreatingDummyObject(struct FCreatorObjectData* ObjectData);
	// �������� �ʴ°��
	
	UFUNCTION()
	bool DeleteDummyObject();
	void MoveDummyObject(FVector2D MousePosition);

	class ASW_CreatorObject* SelectedObject;
	
	UPROPERTY()
	class ASW_CreatorObject* CreatingObject;

	// ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class USW_CreatorWidget> CreatorWidgetFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class USW_CreatorWidget* CreatorWidget;

	TArray<ASW_CreatorObject*> PendingDestroyActors;
};
