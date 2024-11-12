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

    // ���丮 ����
    IFileManager::Get().MakeDirectory(*SavePath, true);

    FViewport* Viewport = GEngine->GameViewport->Viewport;
    if (!Viewport)
    {
        return false;
    }

    // ��ũ���� ĸó
    TArray<FColor> Bitmap;
    FIntVector Size(Viewport->GetSizeXY().X, Viewport->GetSizeXY().Y, 0);

    if (!Viewport->ReadPixels(Bitmap))
    {
        return false;
    }

    // PNG�� ��ȯ�Ͽ� ����
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

    // ���� ���� Ȯ��
    if (!FPaths::FileExists(FilePath))
    {
        return nullptr;
    }

    // ���� ������ �ε�
    TArray<uint8> FileData;
    if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
    {
        return nullptr;
    }

    // �̹��� ���� ��� �ε�
    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

    if (!ImageWrapper.IsValid() || !ImageWrapper->SetCompressed(FileData.GetData(), FileData.Num()))
    {
        return nullptr;
    }

    // �̹��� ������ ���� ����
    TArray<uint8> UncompressedData;
    if (!ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedData))
    {
        return nullptr;
    }

    // �ؽ�ó ����
    UTexture2D* Texture = UTexture2D::CreateTransient(
        ImageWrapper->GetWidth(),
        ImageWrapper->GetHeight(),
        PF_B8G8R8A8
    );

    if (!Texture)
    {
        return nullptr;
    }

    // �ؽ�ó ������ ����
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
