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

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	struct FCreatorObjectData* ObjectData;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ObjectButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ObjectImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ObjectName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class USW_ObjectDragOperation> DragOperationFactory;

	class USW_ObjectDragOperation* DragOp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class USW_DragObjectItemWidget> DragFactory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UTexture2D* MouseObjectTexture;

	void SetObject(struct FCreatorObjectData* data);

	virtual void NativeOnDragDetected(const FGeometry& InGeometry,
		const FPointerEvent& InPointerEvent,
		UDragDropOperation*& OutOperation) override;

	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	
	bool bIsDragging = false;
};
