// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_CreatorObjectSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "../CreatorStorageSubsystem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "SW_ObjectDragOperation.h"
#include "SW_DragObjectItemWidget.h"
#include "../SW_CreatorPlayerController.h"
#include "ViewportClient.h"
#include "Engine/EngineBaseTypes.h"
#include "Components/Widget.h"
#include "UObject/Object.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Layout/Geometry.h"
#include "Framework/Application/SlateApplication.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Widgets/SWidget.h"
#include "Widgets/SViewport.h"
#include "Templates/SharedPointer.h"
#include "Types/SlateVector2.h"
#include "../CreatorFBXSubsystem.h"

void USW_CreatorObjectSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Visible);
	bIsDragging = false;
}

void USW_CreatorObjectSlotWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsDragging)
	{
		// 드래그 중일 때 매 프레임 호출되는 코드
		ASW_CreatorPlayerController* PC = Cast< ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());

		FVector2d LocalPixelMousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		if (PC)
		{
			LocalPixelMousePos.X = FMath::Clamp(LocalPixelMousePos.X * MyGeometry.Scale, (double)TNumericLimits<int32>::Min(), (double)TNumericLimits<int32>::Max());
			LocalPixelMousePos.Y = FMath::Clamp(LocalPixelMousePos.Y * MyGeometry.Scale, (double)TNumericLimits<int32>::Min(), (double)TNumericLimits<int32>::Max());
			// viewport 좌표를 world 좌표로 변환
			PC->MoveDummyObject(LocalPixelMousePos);
		}
	}
}

void USW_CreatorObjectSlotWidget::SetObject(FCreatorObjectData* data)
{
	ObjectData = data;
	// ObjectData = data;
	if (data->CObjectType == 5)
	{
		UCreatorFBXSubsystem* CreatorFBX = GetGameInstance()->GetSubsystem<UCreatorFBXSubsystem>();
		FCreatorFBXMetaData metadata = CreatorFBX->GetMetaData(data->ItemName);
		ObjectName->SetText(FText::FromString(metadata.FBXName));
	}
	else
	{
		ObjectName->SetText(FText::FromString(data->ItemName));
	}

	ObjectImage->SetBrushFromTexture(data->ItemIcon);
}

void USW_CreatorObjectSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InPointerEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InPointerEvent, OutOperation);
	// DragFactory widget 생성
	USW_DragObjectItemWidget* widget = CreateWidget<USW_DragObjectItemWidget>(GetWorld(), DragFactory);
	USW_ObjectDragOperation* dragOperation = NewObject<USW_ObjectDragOperation>();
	
	widget->ObjectImage->SetBrushFromTexture(ObjectData->ItemIcon);
	dragOperation->DefaultDragVisual = widget;
	dragOperation->ItemName = ObjectData->ItemName;

	// 플레이어 컨트롤
	ASW_CreatorPlayerController* controller = Cast<ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());
	if (controller)
	{
		FVector2d LocalPixelMousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		LocalPixelMousePos.X = FMath::Clamp(LocalPixelMousePos.X * InGeometry.Scale, (double)TNumericLimits<int32>::Min(), (double)TNumericLimits<int32>::Max());
		LocalPixelMousePos.Y = FMath::Clamp(LocalPixelMousePos.Y * InGeometry.Scale, (double)TNumericLimits<int32>::Min(), (double)TNumericLimits<int32>::Max());

		controller->CreatingDummyObject(ObjectData);
		controller->MoveDummyObject(LocalPixelMousePos);
		// 마우스 모드 변경
	}

	bIsDragging = true;
	OutOperation = dragOperation;
	DragOp = dragOperation;
}

FReply USW_CreatorObjectSlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
	FEventReply ret = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);

	return ret.NativeReply;
}

void USW_CreatorObjectSlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	// 플레이어 컨트롤
	ASW_CreatorPlayerController* controller = Cast<ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());
	if (controller)
	{
		FVector2D MousePosition = InDragDropEvent.GetScreenSpacePosition();
		FGeometry WidgetGeometry = GetCachedGeometry();

		// UI 위에 있는지 확인
			controller->DeleteDummyObject();

		FInputModeGameAndUI InputMode;
		controller->SetInputMode(InputMode);
	}
	
	DragOp = nullptr;
	bIsDragging = false;
}