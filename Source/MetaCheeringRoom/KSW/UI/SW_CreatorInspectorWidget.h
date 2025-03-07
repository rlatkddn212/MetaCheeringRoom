// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SW_CreatorInspectorWidget.generated.h"

// 델리게이트
DECLARE_DYNAMIC_DELEGATE_OneParam(FChangePosition, FVector, Pos);
DECLARE_DYNAMIC_DELEGATE_OneParam(FChangeRotation, FRotator, Rot);
DECLARE_DYNAMIC_DELEGATE_OneParam(FChangeScale, FVector, Scale);

// 컬러픽커 델리게이트
DECLARE_DYNAMIC_DELEGATE_OneParam(FChangeColorPicker, FLinearColor, Color);

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_CreatorInspectorWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UScrollBox* InspectorScrollBox;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UEditableText* ObjectName;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UEditableText* PosX;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UEditableText* PosY;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UEditableText* PosZ;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UEditableText* RotX;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UEditableText* RotY;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UEditableText* RotZ;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UEditableText* ScaleX;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UEditableText* ScaleY;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UEditableText* ScaleZ;

	// 체크박스
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UCheckBox* VisiableCheckBox;
	
	UFUNCTION()
	void OnObjectNameChanged(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnPositionXChanged(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnPositionYChanged(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnPositionZChanged(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnRotationXChanged(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnRotationYChanged(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnRotationZChanged(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnScaleXChanged(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnScaleYChanged(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnScaleZChanged(const FText& Text, ETextCommit::Type CommitMethod);

	void ChangePositionWidget(FVector Pos);
	void ChangeRotationWidget(FRotator Rot);
	void ChangeScaleWidget(FVector Scale);

	// Change Pos
	UFUNCTION()
	void OnChangePosition(FVector Pos);

	// Change Rot
	UFUNCTION()
	void OnChangeRotation(FRotator Rot);

	// Change Scale
	UFUNCTION()
	void OnChangeScale(FVector Scale);
	
	UFUNCTION(BlueprintCallable)
	void OnColorChanged(FLinearColor Color);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnColorButtonClicked(FLinearColor Color);
	
	UFUNCTION(BlueprintImplementableEvent)
	void CloseColorPicker();

	UFUNCTION()
	void OnChanged();

	void SetObject(class ASW_CreatorObject* Obj);

	FChangePosition ChangePosition;
	FChangeRotation ChangeRotation;
	FChangeScale ChangeScale;
	FChangeColorPicker ChangeColorPicker;

	ASW_CreatorObject* CreatorObject;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class USW_PropertyFloatWidget> PropertyFloatWidgetFactory;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class USW_PropertyBoolWidget> PropertyBoolWidgetFactory;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class USW_PropertyColorWidget> PropertyColorWidgetFactory;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class USW_PropertyEnumWidget> PropertyEnumWidgetFactory;

	TMap<int32, class UUserWidget*> PropertyWidgets;
};
