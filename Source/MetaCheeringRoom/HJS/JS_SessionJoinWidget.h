// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JS_SessionJoinWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UJS_SessionJoinWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime);
	
	UPROPERTY(meta=(BindWidget))
	class UWidgetSwitcher* WS_Menu;

	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_Quit;

	UFUNCTION()
	void MenuSwitching(int32 index);

	// 방 만들기 start-------------------------
	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* CA_MakeSession;
	UPROPERTY(meta=(BindWidget))
	class UScrollBox* SB_Maps;
	UPROPERTY(meta=(BindWidget))
	class UBorder* BD_CreateRoom;
	UPROPERTY(meta=(BindWidget))
	class UBorder* BD_CreateRoomPopup;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_Create;
	UPROPERTY(meta = (BindWidget))
	class UEditableText* ED_RoomName;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_CreateQuit;
	UPROPERTY(meta=(BindWidget))
	class UComboBoxString* CB_Category;
	UFUNCTION()
	void CreateSession();

	void SetupMapData();

	void OnClickSlot(int32 slotIdx);

	int32 SelectIndex = -1;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UJS_HostSlotWidget> SlotFactory;

	float AnimationAlpha = 0.f;
	UPROPERTY(EditAnywhere)
	float AnimationDuration = 0.8f;
	UPROPERTY(EditAnywhere)
	float StartMakeWidgetPosition;
	UPROPERTY(EditAnywhere)
	float TargetOffset = 200.f;
	bool bMakeWidgetAnimating = false;
	void PlayShowMakeSessionAnimation();
	void OnMakeSessionAnimation(float DeltaTime);
	bool bMakePopupAnimating = false;
	void PlayShowMakePopupSessionAnimation();
	void OnMakePopupSessionAnimation(float DeltaTime);

	// 방 만들기 End -----------------------------



	// 방 입장 Start -----------------------------
	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* CA_FindSession;
	
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_All;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_ESports;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_Soccer;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_Idol;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_Talk;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_Refresh;
	UPROPERTY(meta=(BindWidget))
	class UWidgetSwitcher* WS_Category;

	UPROPERTY(meta=(BindWidget))
	class UScrollBox* SB_All;
	UPROPERTY(meta=(BindWidget))
	class UScrollBox* SB_ESports;
	UPROPERTY(meta=(BindWidget))
	class UScrollBox* SB_Soccer;
	UPROPERTY(meta=(BindWidget))
	class UScrollBox* SB_Idol;
	UPROPERTY(meta=(BindWidget))
	class UScrollBox* SB_Talk;

	UPROPERTY(meta=(BindWidget))
	class UBorder* BD_JoinRoom;
	UPROPERTY(meta=(BindWidget))
	class UBorder* BD_JoinPopup;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_JoinBack;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_Join;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_JoinQuit;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UJS_SessionSlotWidget> SessionSlotFactory;
	UPROPERTY(EditAnywhere)
	int32 LobbySelectedIndex = -1;

	UFUNCTION()
	void AddSessionSlotWidget(const FRoomInfo& info);

	void ShowJoinWidget();

	UFUNCTION()
	void CategorySwitching(int32 index);

	UFUNCTION()
	void OnClickedCateAll();
	UFUNCTION()
	void OnClickedCateESports();
	UFUNCTION()
	void OnClickedCateSoccer();
	UFUNCTION()
	void OnClickedCateIdol();
	UFUNCTION()
	void OnClickedCateTalk();
	UFUNCTION()
	void OnClickedRefresh();
	UFUNCTION()
	void OnClickedJoinRoom();
	UFUNCTION()
	void OnClickedJoinQuit();
	UFUNCTION()
	void OnClickedCreateQuit();
	UFUNCTION()
	void OnClickedQuit();
	UPROPERTY()
	class UJS_SessionGameInstanceSubSystem* si;

	bool bJoinPopupAnimating = false;
	void PlayJoinPopupSessionAnimation();
	void OnJoinPopupSessionAnimation(float DeltaTime);

	bool bJoinWidgetAnimating = false;
	void PlayShowJoinSessionAnimation();
	void OnJoinSessionAnimation(float DeltaTime);

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* ChangeBtnEsports;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* ChangeBtnSoccer;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* ChangeBtnIdol;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* ChangeBtnTalk;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* ShowWidget;

	int32 SelectNum = 0;

	void BtnReversePlay();

	// 방 입장 End -------------------------------

};
