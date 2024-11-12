// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HG_RemoteCS.generated.h"

/**
 *
 */

class UButton;
class UCheckBox;
class UEditableText;
class AHG_CheeringStick;

UCLASS()
class METACHEERINGROOM_API UHG_RemoteCS : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// ============바인드 =================
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

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Btn_Commit;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UCheckBox* CB_Bling;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UEditableText* ET_Intesity;

	// ========= 이벤트 함수 =====================
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

	UFUNCTION()
	void OnClickCommit();

	UFUNCTION()
	void OnTextEnter(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnCheckBoxChanged(bool bIsChecked);

	// =============== 변수들 ==============
	TArray<AActor*> CheerSticks;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHG_CheeringStick> CSClass;

	FLinearColor Color;

	float Intensity;

	class AHG_Player* Onwer;

	void SetOnwer(AHG_Player* Value);

	bool bChecked = false;
};

