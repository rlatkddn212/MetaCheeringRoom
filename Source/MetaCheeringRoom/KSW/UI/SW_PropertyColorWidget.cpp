// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_PropertyColorWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SW_CreatorInspectorWidget.h"
#include "../CreatorMapSubsystem.h"
#include "../CreatorObject/SW_CreatorObject.h"

void USW_PropertyColorWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ColorImageButton->OnClicked.AddDynamic(this, &USW_PropertyColorWidget::OnColorButtonClicked);
}

void USW_PropertyColorWidget::SetPropertyValue(int32 id, class UCreatorColorProperty* Property)
{
	PropertyId = id;
	ColorProperty = Property;
	NameText->SetText(FText::FromString(ColorProperty->PropertyName));
	ColorImageButton->WidgetStyle.Normal.TintColor = FSlateColor(ColorProperty->Value);
}

void USW_PropertyColorWidget::SetInspectorWidget(USW_CreatorInspectorWidget* InInspectorWidget)
{
	InspectorWidget = InInspectorWidget;
	
}

void USW_PropertyColorWidget::OnColorChanged(FLinearColor Color)
{
	// 버튼 색깔 변경
	ColorImageButton->WidgetStyle.Normal.TintColor = FSlateColor(Color);
	ColorProperty->Value = Color;
	InspectorWidget->CreatorObject->OnChangeProperty(PropertyId, ColorProperty);
}

void USW_PropertyColorWidget::OnColorButtonClicked()
{
	InspectorWidget->ChangeColorPicker.BindDynamic(this, &USW_PropertyColorWidget::OnColorChanged);
	InspectorWidget->OnColorButtonClicked();
}
