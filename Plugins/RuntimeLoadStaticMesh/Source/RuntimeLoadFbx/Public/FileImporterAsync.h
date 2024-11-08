// Copyright 2021 ls Sun, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Async/TaskGraphInterfaces.h>
#include <Materials/MaterialInterface.h>
#include <Materials/MaterialInstanceDynamic.h>
#include <ProceduralMeshComponent.h>
#include "RLFProgress.h"

/**
 * 
 */

namespace FileIO
{
    RUNTIMELOADFBX_API TWeakObjectPtr<AActor> LoadFile2ProceduralMesh(const FString& filePath
    , bool bAsync = true
    , URLFProgress* fprogress = nullptr
    , TEnumAsByte<enum CalculateCollisionMethod> calCollision = CalculateCollisionMethod::eComplexCollision
    , bool CalTangentIfNone = false
    , TSubclassOf<AActor> actorClass = AActor::StaticClass()
    , UWorld* world=nullptr
    );
}
