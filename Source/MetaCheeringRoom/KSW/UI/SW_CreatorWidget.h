// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SW_CreatorWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_CreatorWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	class ASW_CreatorPlayerController* ControllerReference;

	void OnDragged(bool isDrag) { bIsDrag = isDrag; }

	bool bIsDrag;

	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDragOver( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation ) override;
	bool IsDragOverUI();

	bool IsOverUI = false;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class USW_CreatorHierarchyWidget* CreatorHierarchyWidget;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class USW_CreatorInspectorWidget* CreatorInspectorWidget;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class USW_CreatorMenuWidget* CreatorMenuWidget;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class USW_CreatorObjectWidget* CreatorObjectWidget;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class USW_CreatorModelToolbarWidget* ModelToolbarWidget;

};