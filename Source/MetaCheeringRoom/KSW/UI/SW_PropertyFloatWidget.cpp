// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_PropertyFloatWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "SW_CreatorInspectorWidget.h"
#include "../CreatorObject/SW_CreatorObject.h"

void USW_PropertyFloatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ValueText->OnTextCommitted.AddDynamic(this, &USW_PropertyFloatWidget::OnFloatChanged);
}

void USW_PropertyFloatWidget::SetPropertyValue(int32 id, UCreatorFloatProperty* Property)
{
	PropertyId = id;
	FloatProperty = Property;
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
