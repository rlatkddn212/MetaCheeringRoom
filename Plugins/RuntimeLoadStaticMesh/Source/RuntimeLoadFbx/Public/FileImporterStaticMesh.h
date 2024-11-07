// Copyright 2021 ls Sun, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Engine/StaticMesh.h>
#include "RLFProgress.h"
/**
 * 
 */
namespace FileIO
{
    RUNTIMELOADFBX_API TWeakObjectPtr<AActor> LoadFileStaticMesh(const FString& filePath
        , bool ASync = true
        , URLFProgress* fprogress=nullptr
        , TEnumAsByte<enum CalculateCollisionMethod> calCollision = CalculateCollisionMethod::eComplexCollision
        , bool CalTangentIfNone = false
        , TSubclassOf<AActor> actorClass = AActor::StaticClass()
        , UWorld* world = nullptr);

}