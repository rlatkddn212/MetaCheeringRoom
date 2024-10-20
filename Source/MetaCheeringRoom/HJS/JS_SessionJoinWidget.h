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
	class UButton* BTN_Create;

	UPROPERTY(meta=(BindWidget))
	class USlider* S_PlayerCount;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TEXT_PlayerCount;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* ED_RoomName;

	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_CreateQuit;
	UPROPERTY(meta=(BindWidget))
	class UComboBoxString* CB_Category;

	UFUNCTION()
	void OnSliderValueChanged(float value);

	UFUNCTION()
	void CreateSession();

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
	void OnClickedQuit();
	UPROPERTY()
	class UJS_SessionGameInstanceSubSystem* si;
	// 방 입장 End -------------------------------

};
