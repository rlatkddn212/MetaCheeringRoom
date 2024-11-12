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

	/** 현재 뷰포트의 스크린샷을 캡처하여 파일로 저장합니다 */
    UFUNCTION(BlueprintCallable, Category = "Utils|Screenshot")
    static bool CaptureScreenshot(const FString& FileName, const FString& CustomPath = TEXT(""));

    /** 저장된 스크린샷을 텍스처로 로드합니다 */
    UFUNCTION(BlueprintCallable, Category = "Utils|Screenshot")
    static UTexture2D* LoadScreenshotAsTexture(const FString& FileName, const FString& CustomPath = TEXT(""));

    /** 스크린샷 저장 경로를 가져옵니다 */
    UFUNCTION(BlueprintPure, Category = "Utils|Screenshot")
    static FString GetScreenshotSavedPath(const FString& CustomPath = TEXT(""));

    /** 스크린샷 파일이 존재하는지 확인합니다 */
    UFUNCTION(BlueprintPure, Category = "Utils|Screenshot")
    static bool DoesScreenshotExist(const FString& FileName, const FString& CustomPath = TEXT(""));
};
