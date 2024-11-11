// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_CreatorInspectorWidget.h"
#include "Components/EditableText.h"
#include "../CreatorObject/SW_CreatorObject.h"
#include "Delegates/Delegate.h"
#include "Delegates/DelegateCombinations.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Styling/SlateColor.h"

void USW_CreatorInspectorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ObjectName->OnTextCommitted.AddDynamic(this, &USW_CreatorInspectorWidget::OnObjectNameChanged);
	PosX->OnTextCommitted.AddDynamic(this, &USW_CreatorInspectorWidget::OnPositionXChanged);
	PosY->OnTextCommitted.AddDynamic(this, &USW_CreatorInspectorWidget::OnPositionYChanged);
	PosZ->OnTextCommitted.AddDynamic(this, &USW_CreatorInspectorWidget::OnPositionZChanged);

	RotX->OnTextCommitted.AddDynamic(this, &USW_CreatorInspectorWidget::OnRotationXChanged);
	RotY->OnTextCommitted.AddDynamic(this, &USW_CreatorInspectorWidget::OnRotationYChanged);
	RotZ->OnTextCommitted.AddDynamic(this, &USW_CreatorInspectorWidget::OnRotationZChanged);

	ScaleX->OnTextCommitted.AddDynamic(this, &USW_CreatorInspectorWidget::OnScaleXChanged);
	ScaleY->OnTextCommitted.AddDynamic(this, &USW_CreatorInspectorWidget::OnScaleYChanged);
	ScaleZ->OnTextCommitted.AddDynamic(this, &USW_CreatorInspectorWidget::OnScaleZChanged);
}

void USW_CreatorInspectorWidget::OnChangePosition(FVector Pos)
{
	ChangePosition.ExecuteIfBound(Pos);

	PosX->SetText(FText::FromString(FString::SanitizeFloat(Pos.X)));
	PosY->SetText(FText::FromString(FString::SanitizeFloat(Pos.Y)));
	PosZ->SetText(FText::FromString(FString::SanitizeFloat(Pos.Z)));
}

void USW_CreatorInspectorWidget::OnChangeRotation(FRotator Rot)
{
	ChangeRotation.ExecuteIfBound(Rot);

	RotX->SetText(FText::FromString(FString::SanitizeFloat(Rot.Roll)));
	RotY->SetText(FText::FromString(FString::SanitizeFloat(Rot.Pitch)));
	RotZ->SetText(FText::FromString(FString::SanitizeFloat(Rot.Yaw)));
}

void USW_CreatorInspectorWidget::OnChangeScale(FVector Scale)
{
	ChangeScale.ExecuteIfBound(Scale);

	ScaleX->SetText(FText::FromString(FString::SanitizeFloat(Scale.X)));
	ScaleY->SetText(FText::FromString(FString::SanitizeFloat(Scale.Y)));
	ScaleZ->SetText(FText::FromString(FString::SanitizeFloat(Scale.Z)));
}

void USW_CreatorInspectorWidget::OnColorChanged(FLinearColor Color)
{
	// 버튼 색깔 변경
	ColorImageButton->WidgetStyle.Normal.TintColor = FSlateColor(Color);

	if (CreatorObject)
	{
		//CreatorObject->OnChangeProperty(Color);
	}
}

void USW_CreatorInspectorWidget::OnChanged()
{
	if (CreatorObject)
	{
		OnChangePosition(CreatorObject->GetActorLocation());
		OnChangeRotation(CreatorObject->GetActorRotation());
		OnChangeScale(CreatorObject->GetActorScale3D());
	}
}

void USW_CreatorInspectorWidget::SetObject(ASW_CreatorObject* Obj)
{
	if (Obj == nullptr)
	{
		InspectorScrollBox->SetVisibility(ESlateVisibility::Hidden);
		ColorImageButton->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor::White);
	}
	else
	{
		CreatorObject = Obj;
		InspectorScrollBox->SetVisibility(ESlateVisibility::Visible);
		ObjectName->SetText(FText::FromString(Obj->GetName()));
		ChangePosition.BindDynamic(Obj, &ASW_CreatorObject::OnChangePosition);
		ChangeRotation.BindDynamic(Obj, &ASW_CreatorObject::OnChangeRotation);
		ChangeScale.BindDynamic(Obj, &ASW_CreatorObject::OnChangeScale);
		//ColorImageButton->WidgetStyle.Normal.TintColor = FSlateColor(Obj->GetProperty());

		// 프로퍼티 UI 생성


		OnChanged();
	}
}

void USW_CreatorInspectorWidget::OnObjectNameChanged(const FText& Text, ETextCommit::Type CommitMethod)
{
	// 이름 변경

}

void USW_CreatorInspectorWidget::OnPositionXChanged(const FText& Text, ETextCommit::Type CommitMethod)
{
	float x = FCString::Atof(*Text.ToString());
	float y = FCString::Atof(*PosY->GetText().ToString());
	float z = FCString::Atof(*PosZ->GetText().ToString());

	OnChangePosition(FVector(x, y, z));
}

void USW_CreatorInspectorWidget::OnPositionYChanged(const FText& Text, ETextCommit::Type CommitMethod)
{
	float x = FCString::Atof(*PosX->GetText().ToString());
	float y = FCString::Atof(*Text.ToString());
	float z = FCString::Atof(*PosZ->GetText().ToString());

	OnChangePosition(FVector(x, y, z));
}

void USW_CreatorInspectorWidget::OnPositionZChanged(const FText& Text, ETextCommit::Type CommitMethod)
{
	float x = FCString::Atof(*PosX->GetText().ToString());
	float y = FCString::Atof(*PosY->GetText().ToString());
	float z = FCString::Atof(*Text.ToString());

	OnChangePosition(FVector(x, y, z));
}

void USW_CreatorInspectorWidget::OnRotationXChanged(const FText& Text, ETextCommit::Type CommitMethod)
{
	// (Pitch=2.000000,Yaw=3.000000,Roll=1.000000)	
	// (X=62.702991,Y=1746.899573,Z=709.000000)
	float x = FCString::Atof(*Text.ToString());
	float y = FCString::Atof(*RotY->GetText().ToString());
	float z = FCString::Atof(*RotZ->GetText().ToString());

	OnChangeRotation(FRotator(y, z, x));
}

void USW_CreatorInspectorWidget::OnRotationYChanged(const FText& Text, ETextCommit::Type CommitMethod)
{
	float x = FCString::Atof(*RotX->GetText().ToString());
	float y = FCString::Atof(*Text.ToString());
	float z = FCString::Atof(*RotZ->GetText().ToString());

	OnChangeRotation(FRotator(y, z, x));
}

void USW_CreatorInspectorWidget::OnRotationZChanged(const FText& Text, ETextCommit::Type CommitMethod)
{
	float x = FCString::Atof(*RotX->GetText().ToString());
	float y = FCString::Atof(*RotY->GetText().ToString());
	float z = FCString::Atof(*Text.ToString());

	OnChangeRotation(FRotator(y, z, x));
}

void USW_CreatorInspectorWidget::OnScaleXChanged(const FText& Text, ETextCommit::Type CommitMethod)
{
	float x = FCString::Atof(*Text.ToString());
	float y = FCString::Atof(*ScaleY->GetText().ToString());
	float z = FCString::Atof(*ScaleZ->GetText().ToString());

	OnChangeScale(FVector(x, y, z));
}

void USW_CreatorInspectorWidget::OnScaleYChanged(const FText& Text, ETextCommit::Type CommitMethod)
{
	float x = FCString::Atof(*ScaleX->GetText().ToString());
	float y = FCString::Atof(*Text.ToString());
	float z = FCString::Atof(*ScaleZ->GetText().ToString());

	OnChangeScale(FVector(x, y, z));
}

void USW_CreatorInspectorWidget::OnScaleZChanged(const FText& Text, ETextCommit::Type CommitMethod)
{
	float x = FCString::Atof(*ScaleX->GetText().ToString());
	float y = FCString::Atof(*ScaleY->GetText().ToString());
	float z = FCString::Atof(*Text.ToString());

	OnChangeScale(FVector(x, y, z));
}
