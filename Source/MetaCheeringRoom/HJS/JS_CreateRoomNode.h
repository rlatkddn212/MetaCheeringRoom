// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JS_CreateRoomNode.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UJS_CreateRoomNode : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* IMG_Thumbnail;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TEXT_MapName;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* BTN_Hosting;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* BTN_Modify;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* BTN_Remove;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* BTN_Select;

	UFUNCTION()
	void OnSelect();
	UFUNCTION()
	void OnHosting();
	UFUNCTION()
	void OnRemove();
	UFUNCTION()
	void OnModify();

	struct FCreatorMapMetaData* MapMetaData;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* SelectAnim;

	class UJS_CreateRoomWidget* Parent;

	void SetupInfo(struct FCreatorMapMetaData* metaData, class UJS_CreateRoomWidget* parent, int32 idx);

	int32 slotIdx;
};
