// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Templates/SharedPointer.h"
#include "SW_CreatorHierarchyItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API USW_CreatorHierarchyItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetItem(const TSharedPtr<struct FCreatorObject>& InCreatorObject, int32 depth);

	TSharedPtr<struct FCreatorObject> CreatorObject;

	// spacer
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class USpacer* HierarchySpacer;

	// 이미지
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UImage* IconImage;

	// 텍스트
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* NameText;
};
