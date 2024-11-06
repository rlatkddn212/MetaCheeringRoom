// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JS_Screen.h"
#include "VideoWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UVideoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
	UPROPERTY(meta=(BindWIdget))
	class UButton* BTN_CategoryLive;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_CategoryVOD;
	UPROPERTY(meta=(BindWidget))
	class UScrollBox* SB_Live;
	UPROPERTY(meta=(BindWidget))
	class UScrollBox* SB_VOD;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_Quit;
	UPROPERTY(meta=(BindWidget))
	class UBorder* BD_VideoList;

	UFUNCTION()
	void OnClickQuitBtn();

	UFUNCTION()
	void SettingLive();
	UFUNCTION()
	void SettingVOD();

	void SettingLiveInfo(TArray<FVedioInfo>& LiveInfoList);

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UVideoList> VideoListFactory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UVideoNode> VideoNodeFactory;

	bool IsEmoji(char32_t Codepoint);

	FString RemoveEmojis(const FString& Input);

	bool IsHighSurrogate(TCHAR Char);

	bool IsLowSurrogate(TCHAR Char);

	char32_t DecodeSurrogatePair(TCHAR HighSurrogate, TCHAR LowSurrogate);

	bool bIsAnimating = false;
	float AnimationAlpha = 0.f;
	UPROPERTY(EditAnywhere)
	float AnimationDuration = 0.8f;
	UPROPERTY(EditAnywhere)
	float StartPosition;
	UPROPERTY(EditAnywhere)
	float TargetOffset;
	void PlayShowAnimation();
	void OnAnimation(float DeltaTime);

	UPROPERTY(meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* ChangeBtnLive;
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* ChangeBtnVOD;
	bool bLive = true;
};
