// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Templates/SharedPointer.h"
#include "SW_CreatorHierarchyItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_CreatorHierarchyItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetItem(const TSharedPtr<struct FCreatorObject>& InCreatorObject, int32 depth);

	TSharedPtr<struct FCreatorObject> CreatorObject;

	// spacer
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class USpacer* HierarchySpacer;

	// �̹���
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UImage* IconImage;

	// �ؽ�Ʈ
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* NameText;

	virtual void NativeConstruct() override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry,
		const FPointerEvent& InPointerEvent,
		UDragDropOperation*& OutOperation) override;

	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class USW_HierarchyDragOperation> DragOperationFactory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class USW_DragHierarchyItemWidget> DragFactory;

	bool bIsDragging = false;
	class ASW_CreatorPlayerController* PC;
};
