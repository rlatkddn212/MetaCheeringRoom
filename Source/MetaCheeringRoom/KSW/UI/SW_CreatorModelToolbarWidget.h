// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../SW_CreatorPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Styling/SlateTypes.h"
#include "SlateBasics.h"
#include "SW_CreatorModelToolbarWidget.generated.h"

// enum 전달 델리게이트
DECLARE_DELEGATE_OneParam(FOnCreatorToolStateChanged, enum ECreatorToolState);

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_CreatorModelToolbarWidget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* SelectionButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* PositionButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* RotationButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ScaleButton;

	FOnCreatorToolStateChanged OnCreatorToolStateChanged;
	
	// click event
	UFUNCTION()
	void OnSelectionButtonClicked();

	UFUNCTION()
	void OnPositionButtonClicked();

	UFUNCTION()
	void OnRotationButtonClicked();

	UFUNCTION()
	void OnScaleButtonClicked();

	void SetCreatorToolState(ECreatorToolState state);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonStyle")
	FButtonStyle DefaultButtonStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonStyle")
	FButtonStyle EnableButtonStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonStyle")
	FButtonStyle HoverButtonStyle;
};
