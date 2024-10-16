// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SW_CreatorObjectSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_CreatorObjectSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ObjectButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ObjectImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ObjectName;

	void SetObject(struct FCreatorObjectData* data);


	virtual void NativeOnDragDetected(const FGeometry& InGeometry,
		const FPointerEvent& InPointerEvent,
		UDragDropOperation*& OutOperation) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry,
		const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
};
