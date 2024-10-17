// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HG_ItemBase.h"
#include "HG_ItemPurchaseWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UHG_GameInstance;

/**
 *
 */
UCLASS()
class METACHEERINGROOM_API UHG_ItemPurchaseWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* Img_ItemImg;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* TB_ItemName;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* TB_ItemPrice;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_Purchase;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_Exit;

	
	UFUNCTION()
	void OnClickPurchaseButton();

	UFUNCTION()
	void OnClickExitButton();

	
	void SetItemInfo(FItemData ItemInfo);
	
	void SetOwner(class AHG_Player* Value);

	AHG_Player* Owner;

	FItemData LookingItemData;

	UHG_GameInstance* GI;
};
