// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UUtilBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/** ���� ����Ʈ�� ��ũ������ ĸó�Ͽ� ���Ϸ� �����մϴ� */
    UFUNCTION(BlueprintCallable, Category = "Utils|Screenshot")
    static bool CaptureScreenshot(const FString& FileName, const FString& CustomPath = TEXT(""));

    /** ����� ��ũ������ �ؽ�ó�� �ε��մϴ� */
    UFUNCTION(BlueprintCallable, Category = "Utils|Screenshot")
    static UTexture2D* LoadScreenshotAsTexture(const FString& FileName, const FString& CustomPath = TEXT(""));

    /** ��ũ���� ���� ��θ� �����ɴϴ� */
    UFUNCTION(BlueprintPure, Category = "Utils|Screenshot")
    static FString GetScreenshotSavedPath(const FString& CustomPath = TEXT(""));

    /** ��ũ���� ������ �����ϴ��� Ȯ���մϴ� */
    UFUNCTION(BlueprintPure, Category = "Utils|Screenshot")
    static bool DoesScreenshotExist(const FString& FileName, const FString& CustomPath = TEXT(""));
};
