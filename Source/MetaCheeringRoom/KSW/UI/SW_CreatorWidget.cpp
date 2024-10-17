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
	 

	//if (bIsDrag)
	//{
	//	// 드래그 중일때만 이벤트 처리
	//	if (ControllerReference)
	//	{

	//		//InDragDropEvent.GetScreenSpacePosition() Log
	//		UE_LOG(LogTemp, Warning, TEXT("ScreenSpacePosition : %s"), *InDragDropEvent.GetScreenSpacePosition().ToString());
	//		// Screen Space postion to mouse position
	//		FVector2D MousePos = InDragDropEvent.GetScreenSpacePosition();
	//		FVector2D LocalPixelMousePos = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
	//		LocalPixelMousePos.X = FMath::Clamp(LocalPixelMousePos.X * InGeometry.Scale, (double)TNumericLimits<int32>::Min(), (double)TNumericLimits<int32>::Max());
	//		LocalPixelMousePos.Y = FMath::Clamp(LocalPixelMousePos.Y * InGeometry.Scale, (double)TNumericLimits<int32>::Min(), (double)TNumericLimits<int32>::Max());
	//		
	//		// Mouse Position Log
	//		UE_LOG(LogTemp, Warning, TEXT("Mouse Position : %s"), *MousePos.ToString());

	//		ControllerReference->MoveDummyObject(LocalPixelMousePos);
	//	}
	//}

	return true;
}

bool USW_CreatorWidget::IsDragOverUI()
{
	return IsOverUI;
}

