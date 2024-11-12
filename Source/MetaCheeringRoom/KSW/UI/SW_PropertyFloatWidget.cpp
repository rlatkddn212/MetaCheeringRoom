// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_PropertyFloatWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "SW_CreatorInspectorWidget.h"
#include "../CreatorObject/SW_CreatorObject.h"
#include "Components/CheckBox.h"


void USW_PropertyFloatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ValueText->OnTextCommitted.AddDynamic(this, &USW_PropertyFloatWidget::OnFloatChanged);
	bCheckBox->OnCheckStateChanged.AddDynamic(this, &USW_PropertyFloatWidget::OnCheckChanged);
}

void USW_PropertyFloatWidget::OnCheckChanged(bool bIsChecked)
{
	if (InspectorWidget)
	{
		if (FloatProperty)
		{
			FloatProperty->bUse = bIsChecked;
			ValueText->SetIsEnabled(FloatProperty->bUse);
			InspectorWidget->CreatorObject->OnChangeProperty(PropertyId, FloatProperty);
		}
	}
}

void USW_PropertyFloatWidget::SetPropertyValue(int32 id, UCreatorFloatProperty* Property)
{
	PropertyId = id;
	FloatProperty = Property;

	// ÇöÀç°ª Property->Value;
	if (Property->bCheckBox)
	{
		bCheckBox->SetVisibility(ESlateVisibility::Visible);
		bCheckBox->SetCheckedState(Property->bUse ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		ValueText->SetIsEnabled(Property->bUse);
	}
	else
	{
		bCheckBox->SetVisibility(ESlateVisibility::Hidden);
	}

	NameText->SetText(FText::FromString(FloatProperty->PropertyName));
	ValueText->SetText(FText::FromString(FString::SanitizeFloat(FloatProperty->Value)));
}

void USW_PropertyFloatWidget::OnFloatChanged(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (InspectorWidget)
	{
		float Value = FCString::Atof(*Text.ToString());
		if (FloatProperty)
		{
			FloatProperty->Value = Value;
			InspectorWidget->CreatorObject->OnChangeProperty(PropertyId, FloatProperty);
		}
	}
}
