// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_PropertyEnumWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "../CreatorMapSubsystem.h"
#include "SW_CreatorInspectorWidget.h"
#include "../CreatorObject/SW_CreatorObject.h"
#include "Components/CheckBox.h"

void USW_PropertyEnumWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ComboValueString->OnSelectionChanged.AddDynamic(this, &USW_PropertyEnumWidget::OnEnumChanged);
	bCheckBox->OnCheckStateChanged.AddDynamic(this, &USW_PropertyEnumWidget::OnCheckChanged);
}

void USW_PropertyEnumWidget::OnCheckChanged(bool bIsChecked)
{
	if (InspectorWidget)
	{
		if (EnumProperty)
		{
			EnumProperty->bUse = bIsChecked;
			ComboValueString->SetIsEnabled(EnumProperty->bUse);
			InspectorWidget->CreatorObject->OnChangeProperty(PropertyId, EnumProperty);
		}
	}
}

void USW_PropertyEnumWidget::SetPropertyValue(int32 id, UCreatorEnumProperty* Property)
{
	PropertyId = id;
	EnumProperty = Property;

	NameText->SetText(FText::FromString(EnumProperty->PropertyName));

	// 현재값 Property->Value;
	if (Property->bCheckBox)
	{
		bCheckBox->SetVisibility(ESlateVisibility::Visible);
		bCheckBox->SetCheckedState(Property->bUse ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		ComboValueString->SetIsEnabled(Property->bUse);
	}
	else
	{
		bCheckBox->SetVisibility(ESlateVisibility::Hidden);
	}

	// 반복문을 돌면서 넣는다. Property->EnumValues
	for (int32 i = 0; i < Property->EnumValues.Num(); i++)
	{
		ComboValueString->AddOption(Property->EnumValues[i]);
	}

	// 현재 값으로 세팅
	ComboValueString->SetSelectedOption(Property->Value);
}

void USW_PropertyEnumWidget::OnEnumChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	// 선택된 아이템
	if (InspectorWidget)
	{
		if (EnumProperty)
		{
			EnumProperty->Value = SelectedItem;
			InspectorWidget->CreatorObject->OnChangeProperty(PropertyId, EnumProperty);
		}
	}
}
