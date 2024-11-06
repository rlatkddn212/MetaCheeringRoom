// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SW_CreatorObjectWidget.generated.h"

// ป๓ลย
UENUM(BlueprintType)
enum class ECreatorObjectTabState : uint8
{
	ShapeObject,
	SportsObject,
	LightObject,
	EffectObject,
	FBXObject,
};

DECLARE_DELEGATE_OneParam(FOnCreatorObjectTabStateChanged, enum ECreatorObjectTabState);
/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_CreatorObjectWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USW_CreatorObjectSlotWidget> SlotFactory;

	/*UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TArray<class USW_CreatorObjectSlotWidget*> SlotWidgets;*/
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UBorder* ShapeBorder;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UBorder* SportsBorder;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UBorder* LightBorder;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UBorder* EffectBorder;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UBorder* FBXBorder;

	// ObjectScrollBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* ShapeObjectScrollBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* SportsObjectScrollBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* LightObjectScrollBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* EffectObjectScrollBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* FBXObjectScrollBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ShapeObjectButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* SportsObjectButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* LightObjectButton;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* EffectObjectButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* FBXObjectButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ImportButton;

	FOnCreatorObjectTabStateChanged OnCreatorObjectTabStateChanged;
	
	UFUNCTION()
	void OnImportButtonClicked();

	// click event
	UFUNCTION()
	void OnShapeObjectButtonClicked();

	UFUNCTION()
	void OnSportsObjectButtonClicked();

	UFUNCTION()
	void OnLightObjectButtonClicked();

	UFUNCTION()
	void OnEffectObjectButtonClicked();

	UFUNCTION()
	void OnFBXObjectButtonClicked();

	void SetCreatorObjectTabState(ECreatorObjectTabState state);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonStyle")
	FButtonStyle DefaultButtonStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonStyle")
	FButtonStyle EnableButtonStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonStyle")
	FButtonStyle HoverButtonStyle;
};
