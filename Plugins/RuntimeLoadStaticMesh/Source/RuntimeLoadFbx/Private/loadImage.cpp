// Copyright 2021 ls Sun, Inc. All Rights Reserved.


#include "loadImage.h"
#include <ImageUtils.h>
#include <Engine/TextureRenderTarget2D.h>
#include <Misc/MessageDialog.h>
#include <HAL/PlatformFilemanager.h>
#include <Misc/FileHelper.h>
#include <Misc/FeedbackContext.h>
#include <Modules/ModuleManager.h>
#include <IImageWrapperModule.h>


loadImage::loadImage()
{
}

loadImage::~loadImage()
{
}

UTexture* loadImage::Img2Texture(const FString& ImagePath)
{
    return FImageUtils::ImportFileAsTexture2D(ImagePath);
}
