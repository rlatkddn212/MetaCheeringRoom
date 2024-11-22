// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HG_CustomUI.generated.h"


class UButton;
class UTextBlock;
class UImage;
/**
 *
 */
UCLASS()
class METACHEERINGROOM_API UHG_CustomUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void virtual NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UTexture2D*> PNGS;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_Next;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_Commit;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* TB_Target_Color;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* TB_Printing;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* Img_Printing;

	UFUNCTION()
	void SetNextImage();

	UFUNCTION()
	void SetCustomInfo();

	int32 CurIdx = 0;
	int32 Count = 0;
	FLinearColor SelectedColor;

	FLinearColor ClothColor;
	FLinearColor ClothHemColor;
	FLinearColor HairColor;
	FLinearColor EyesColor;
	FLinearColor HairPinColor;
	UTexture2D* ClothPrinting;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_Red;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_Orange;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_Yellow;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_Green;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_Blue;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_Sodomy;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_Purple;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_White;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_Black;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_Grey;

	UFUNCTION()
	void OnClickRed();

	UFUNCTION()
	void OnClickOrange();

	UFUNCTION()
	void OnClicYellow();

	UFUNCTION()
	void OnClickGreen();

	UFUNCTION()
	void OnClickBlue();

	UFUNCTION()
	void OnClickSodomy();

	UFUNCTION()
	void OnClickPurple();

	UFUNCTION()
	void OnClickWhite();

	UFUNCTION()
	void OnClickBlack();

	UFUNCTION()
	void OnClickGrey();

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Appear;

	void PlayAppearAnimation(bool Play_Forward);
};
