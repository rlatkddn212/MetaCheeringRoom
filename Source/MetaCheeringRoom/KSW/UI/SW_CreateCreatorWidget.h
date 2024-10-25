// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SW_CreateCreatorWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_CreateCreatorWidget : public UUserWidget
{
	GENERATED_BODY()
	
	void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* BTN_Quit;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UScrollBox* SB_Maps;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* BTN_Create;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* BTN_Back;

	// Create
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* BD_CreateLevel;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UEditableText* ED_LevelName;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* BTN_CreateLevel;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* BTN_BackLevel;

	// Change
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* BD_ChangeLevel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* BTN_BackChange;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* BTN_Change;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* BTN_Remove;
	
	void SetupInfo();

	// ButtonEvent
	UFUNCTION()
	void OnCreate();
	
	UFUNCTION()
	void OnBack();

	// Create
	UFUNCTION()
	void OnCreateLevel();

	UFUNCTION()
	void OnBackLevel();

	// Change
	UFUNCTION()
	void OnBackChange();
	
	UFUNCTION()
	void OnChange();

	UFUNCTION()
	void OnRemove();

	void OnClickSlot(int32 slotIdx);

	UPROPERTY(EditAnywhere)
	FName LevelName;
	FString SelectMapName;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class USW_CreateCreatorSlotWidget> SlotFactory;

	TArray<class USW_CreateCreatorSlotWidget*> SlotWidgets;
	int32 SelectSlotIdx;
};
