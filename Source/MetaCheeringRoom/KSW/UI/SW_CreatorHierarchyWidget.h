// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SW_CreatorHierarchyWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_CreatorHierarchyWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	virtual void NativeConstruct() override;
	
	void ReloadWidget(const TArray<TSharedPtr<struct FCreatorObject>>& InCreatorObjects, int32 depth);
	void ReloadItem();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USW_CreatorHierarchyItemWidget> SlotFactory;

	// ObjectScrollBox;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UScrollBox* ObjectScrollBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<class USW_CreatorHierarchyItemWidget*> SlotWidgets;
};
