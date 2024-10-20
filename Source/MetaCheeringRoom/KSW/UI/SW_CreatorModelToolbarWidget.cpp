// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_CreatorModelToolbarWidget.h"
#include "Components/Button.h"

void USW_CreatorModelToolbarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SelectionButton->OnClicked.AddDynamic(this, &USW_CreatorModelToolbarWidget::OnSelectionButtonClicked);
	PositionButton->OnClicked.AddDynamic(this, &USW_CreatorModelToolbarWidget::OnPositionButtonClicked);
	RotationButton->OnClicked.AddDynamic(this, &USW_CreatorModelToolbarWidget::OnRotationButtonClicked);
	ScaleButton->OnClicked.AddDynamic(this, &USW_CreatorModelToolbarWidget::OnScaleButtonClicked);
}

void USW_CreatorModelToolbarWidget::OnSelectionButtonClicked()
{
    // 상태 변경실행
	if (OnCreatorToolStateChanged.IsBound())
		OnCreatorToolStateChanged.Execute(ECreatorToolState::Selection);

	//SetCreatorToolState(ECreatorToolState::Selection);
}

void USW_CreatorModelToolbarWidget::OnPositionButtonClicked()
{
	// 상태 변경실행
	if (OnCreatorToolStateChanged.IsBound())
		OnCreatorToolStateChanged.Execute(ECreatorToolState::Position);

	//SetCreatorToolState(ECreatorToolState::Position);
}

void USW_CreatorModelToolbarWidget::OnRotationButtonClicked()
{
	// 상태 변경실행
	if (OnCreatorToolStateChanged.IsBound())
		OnCreatorToolStateChanged.Execute(ECreatorToolState::Rotation);

	//SetCreatorToolState(ECreatorToolState::Rotation);
}

void USW_CreatorModelToolbarWidget::OnScaleButtonClicked()
{
	// 상태 변경실행
	if (OnCreatorToolStateChanged.IsBound())
		OnCreatorToolStateChanged.Execute(ECreatorToolState::Scale);

	//SetCreatorToolState(ECreatorToolState::Scale);
}


void USW_CreatorModelToolbarWidget::SetCreatorToolState(ECreatorToolState state)
{
	SelectionButton->SetStyle(DefaultButtonStyle);
	PositionButton->SetStyle(DefaultButtonStyle);
	RotationButton->SetStyle(DefaultButtonStyle);
	ScaleButton->SetStyle(DefaultButtonStyle);

	if (state == ECreatorToolState::Selection)
	{
		SelectionButton->SetStyle(EnableButtonStyle);
	}
	else if (state == ECreatorToolState::Position)
	{
		PositionButton->SetStyle(EnableButtonStyle);
	}
	else if (state == ECreatorToolState::Rotation)
	{
		RotationButton->SetStyle(EnableButtonStyle);
	}
	else if (state == ECreatorToolState::Scale)
	{
		ScaleButton->SetStyle(EnableButtonStyle);
	}
}
