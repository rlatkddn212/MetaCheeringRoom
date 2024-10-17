// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_CreatorWidget.h"
#include "Ksw/SW_CreatorPlayerController.h"
#include "GameFramework/Controller.h"

void USW_CreatorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// PlayerController 가져오기
	//ControllerReference = Cast<ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());
}

void USW_CreatorWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	IsOverUI = true;
}

void USW_CreatorWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	IsOverUI = false;
}

bool USW_CreatorWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	 

	return true;
}

bool USW_CreatorWidget::IsDragOverUI()
{
	return IsOverUI;
}

