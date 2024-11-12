// Fill out your copyright notice in the Description page of Project Settings.


#include "Util/UtilBlueprintFunctionLibrary.h"
#include "ImageUtils.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Modules/ModuleManager.h"
#include "Engine/Texture2D.h"

bool UUtilBlueprintFunctionLibrary::CaptureScreenshot(const FString& FileName, const FString& CustomPath /*= TEXT("")*/)
{
    if (!GEngine || !GEngine->GameViewport)
    {
        return false;
    }

    FString SavePath = GetScreenshotSavedPath(CustomPath);
    FString FilePath = SavePath / FileName + TEXT(".png");

    // 디렉토리 생성
    IFileManager::Get().MakeDirectory(*SavePath, true);

    FViewport* Viewport = GEngine->GameViewport->Viewport;
    if (!Viewport)
    {
        return false;
    }

    // 스크린샷 캡처
    TArray<FColor> Bitmap;
    FIntVector Size(Viewport->GetSizeXY().X, Viewport->GetSizeXY().Y, 0);

    if (!Viewport->ReadPixels(Bitmap))
    {
        return false;
    }

    // PNG로 변환하여 저장
    TArray<uint8> CompressedData;
    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

    if (ImageWrapper.IsValid() && ImageWrapper->SetRaw(Bitmap.GetData(), Bitmap.Num() * sizeof(FColor), Size.X, Size.Y, ERGBFormat::BGRA, 8))
    {
        CompressedData = ImageWrapper->GetCompressed();
        return FFileHelper::SaveArrayToFile(CompressedData, *FilePath);
    }

    return false;
}

UTexture2D* UUtilBlueprintFunctionLibrary::LoadScreenshotAsTexture(const FString& FileName, const FString& CustomPath /*= TEXT("")*/)
{
    FString FilePath = GetScreenshotSavedPath(CustomPath) / FileName + TEXT(".png");

    // 파일 존재 확인
    if (!FPaths::FileExists(FilePath))
    {
        return nullptr;
    }

    // 파일 데이터 로드
    TArray<uint8> FileData;
    if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
    {
        return nullptr;
    }

    // 이미지 래퍼 모듈 로드
    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

    if (!ImageWrapper.IsValid() || !ImageWrapper->SetCompressed(FileData.GetData(), FileData.Num()))
    {
        return nullptr;
    }

    // 이미지 데이터 압축 해제
    TArray<uint8> UncompressedData;
    if (!ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedData))
    {
        return nullptr;
    }

    // 텍스처 생성
    UTexture2D* Texture = UTexture2D::CreateTransient(
        ImageWrapper->GetWidth(),
        ImageWrapper->GetHeight(),
        PF_B8G8R8A8
    );

    if (!Texture)
    {
        return nullptr;
    }

    // 텍스처 데이터 설정
    void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
    FMemory::Memcpy(TextureData, UncompressedData.GetData(), UncompressedData.Num());
    Texture->GetPlatformData()->Mips[0].BulkData.Unlock();
    Texture->UpdateResource();

    return Texture;
}

FString UUtilBlueprintFunctionLibrary::GetScreenshotSavedPath(const FString& CustomPath /*= TEXT("")*/)
{
    if (!CustomPath.IsEmpty())
    {
        return CustomPath;
    }
    return FPaths::ProjectSavedDir() / TEXT("Screenshots");
}

bool UUtilBlueprintFunctionLibrary::DoesScreenshotExist(const FString& FileName, const FString& CustomPath /*= TEXT("")*/)
{
    FString FilePath = GetScreenshotSavedPath(CustomPath) / FileName + TEXT(".png");
    return FPaths::FileExists(FilePath);
}
