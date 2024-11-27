// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JS_ChattingWidget.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UJS_ChattingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere)
	int32 MAX_TEXT_LEN = 15;

	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	class UEditableText* ED_Chatting;

	UFUNCTION()
	void OnChatChange(const FText& text);

	UFUNCTION(BlueprintCallable)
	void ChatInit();

	UFUNCTION()
	void OnCommitText(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION(BlueprintCallable)
	void AddChat(const FString& id, const FText& text, bool bAuto);

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UJS_ChatNode> ChatNodeFactory;

	UPROPERTY(meta=(BindWidget))
	class UScrollBox* SB_ChatBox;
	UPROPERTY(meta=(BindWidget))
	class UBorder* BD_ChatBar;
	UPROPERTY(meta=(BindWidget))
	class UButton* Btn_ChatCancel;

	UFUNCTION()
	void OnClickedChatCancel();

	FTimerHandle ChatTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bChat = false;

	void OnAutoChat();
	TArray<FString> IDList;
	TArray<FString> CommentList;
	void InitDummychat();

	UFUNCTION()
	void DummyChat();
};
