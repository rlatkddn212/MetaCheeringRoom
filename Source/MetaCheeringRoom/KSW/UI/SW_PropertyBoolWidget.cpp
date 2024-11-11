// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_PropertyBoolWidget.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "../CreatorMapSubsystem.h"
#include "SW_CreatorInspectorWidget.h"
#include "../CreatorObject/SW_CreatorObject.h"

void USW_PropertyBoolWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CheckBoxValue->OnCheckStateChanged.AddDynamic(this, &USW_PropertyBoolWidget::OnBoolChanged);
}

void USW_PropertyBoolWidget::SetPropertyValue(int32 id, UCreatorBoolProperty* Property)
{
	PropertyId = id;
	BoolProperty = Property;
	
	NameText->SetText(FText::FromString(BoolProperty->PropertyName));
	CheckBoxValue->SetCheckedState(BoolProperty->Value ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

void USW_PropertyBoolWidget::OnBoolChanged(bool bIsChecked)
{
	if (InspectorWidget)
	{
		BoolProperty->Value = bIsChecked;
		InspectorWidget->CreatorObject->OnChangeProperty(PropertyId, BoolProperty);
	}
}
