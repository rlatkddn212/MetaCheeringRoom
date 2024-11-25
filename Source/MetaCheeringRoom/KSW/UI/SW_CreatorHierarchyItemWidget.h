// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Templates/SharedPointer.h"
#include "SW_CreatorHierarchyItemWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnHierarchyItemClicked, class ASW_CreatorObject*, CreatorObject);

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_CreatorHierarchyItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetItem(class ASW_CreatorObject* CreatorObject, int32 depth);

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

	UFUNCTION()
	void OnSelected(bool isSelected);

	UFUNCTION()
	void OnClickExpandedButton();

	FOnHierarchyItemClicked OnHierarchyItemClicked;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* HierarchyItemButton;

	// spacer
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class USpacer* HierarchySpacer;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* ExpandedButton;

	// 이미지
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UImage* IconImage;

	// 텍스트
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* NameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonStyle")
	FButtonStyle DefaultButtonStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonStyle")
	FButtonStyle ClickedButtonStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonStyle")
	FButtonStyle HoveredButtonStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExpandedButton")
	FButtonStyle DefaultExpandedButtonStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExpandedButton")
	FButtonStyle ClickedExpandedButtonStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExpandedButton")
	FButtonStyle HoveredExpandedButtonStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class USW_HierarchyDragOperation> DragOperationFactory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class USW_DragHierarchyItemWidget> DragFactory;

	bool bIsDragging = false;
	UPROPERTY()
	class ASW_CreatorPlayerController* PC;
	UPROPERTY()
	class ASW_CreatorObject* HierarchyCreatorObject;
};
