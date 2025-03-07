// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JS_CreateRoomWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UJS_CreateRoomWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime);

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* ShowWidget;
	UPROPERTY()
	class UJS_SessionGameInstanceSubSystem* si;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_Quit;
	UFUNCTION()

	void PlayShowWidgetAnimation();
	UFUNCTION()
	void OnClickedQuit();
	// 방 호스팅 start-------------------------
	UPROPERTY(meta=(BindWidget))
	class UScrollBox* SB_Maps;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_Hosting;
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ED_RoomName;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_HostingCancel;
	UPROPERTY(meta=(BindWidget))
	class UComboBoxString* CB_Category;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* HostingLevelPopup;
	UFUNCTION()
	void CreateSession();

	void SetupMapData();

	void OnClickSlot(int32 slotIdx);

	int32 SelectIndex = -1;

	void OnClickHosting();

	void OnClickModify();

	void OnClickRemove();
	UFUNCTION()
	void OnClickedCreateQuit();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UJS_CreateRoomNode> SlotFactory;

	// 방 호스팅 End -----------------------------
	

	// 방 만들기, 수정, 삭제 Start ----------------------------
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* CreateLevelPopup;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* DeleteLevelPopup;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* ModifyLevelPopup;
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ED_LevelName;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_CreateCancel;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_CreateLevel;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_DeleteCancel;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_Delete;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_CreateRoom;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_Modify;
	UPROPERTY(meta=(BindWidget))
	class UButton* BTN_ModifyCancel;
	UFUNCTION()
	void OnClickedCreateRoom();
	UFUNCTION()
	void OnClickedCreateCancel();
	UFUNCTION()
	void OnClickedCreateLevel();
	UFUNCTION()
	void OnClickedDeleteCancel();
	UFUNCTION()
	void OnClickedDelete();
	UFUNCTION()
	void OnClickedModifyCancel();
	UFUNCTION()
	void OnClickedModify();

	UPROPERTY(EditAnywhere)
	FName LevelName = TEXT("KswTestMap");

	// 방 만들기, 수정, 삭제 End ------------------------------
};
