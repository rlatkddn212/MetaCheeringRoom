// Copyright 2021 ls Sun, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Engine/Texture2D.h>

/**
 * 
 */


class loadImage
{
public:
	loadImage();
	~loadImage();
    static UTexture *Img2Texture(const FString &sImgPath);
};
