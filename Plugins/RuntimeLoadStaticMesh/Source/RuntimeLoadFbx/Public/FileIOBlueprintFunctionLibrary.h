// Copyright 2021 ls Sun, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RLFProgress.h"
#include "FileIOBlueprintFunctionLibrary.generated.h"

/**
 * 
 */



UCLASS()
class RUNTIMELOADFBX_API UFileIOBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

        /// <summary>
         /// Import fbx file
         /// </summary>
         /// <param name="filePath">The path of fbx file</param>
         /// <param name="fprogress">Progress callback also see the function CreateProgress</param>
         /// <param name="calCollision">Calculate collision type</param>
         /// <param name="actorClass">Generate actor class ,null is also set to AActor::StaticClass()</param>
         /// <param name="world">Which world to spawn the root actor</param>
         /// <returns>The result actor</returns>
    UFUNCTION(BlueprintCallable, Category = "RuntimeLoadFbx|Async")
        static  AActor* LoadFileAsync2StaticMeshActor(const FString& filePath
            , URLFProgress*fprogress = nullptr
            , TEnumAsByte<enum CalculateCollisionMethod> calCollision = CalculateCollisionMethod::eComplexCollision
            ,bool CalTangentIfNone = false
         ,TSubclassOf<AActor> actorClass = nullptr
        , UWorld* world = nullptr
    );

    UFUNCTION(BlueprintCallable, Category = "RuntimeLoadFbx|Async")
        static  AActor* LoadFileAsync2ProceduralMeshActor(const FString& filePath
            , URLFProgress* fprogress = nullptr
            , TEnumAsByte<enum CalculateCollisionMethod> calCollision = CalculateCollisionMethod::eComplexCollision
            , bool CalTangentIfNone = false
            , TSubclassOf<AActor> actorClass = nullptr
            , UWorld* world = nullptr
    );

    UFUNCTION(BlueprintCallable, Category = "RuntimeLoadFbx|Sync")
    static  AActor* LoadFileSync2StaticMeshActor(const FString& filePath
        , URLFProgress* fprogress = nullptr
        , TEnumAsByte<enum CalculateCollisionMethod> calCollision = CalculateCollisionMethod::eComplexCollision
        , bool CalTangentIfNone = false
        , TSubclassOf<AActor> actorClass = nullptr
        , UWorld* world = nullptr
    );

    UFUNCTION(BlueprintCallable, Category = "RuntimeLoadFbx|Sync")
    static  AActor* LoadFileSync2ProceduralMeshActor(const FString& filePath
        , URLFProgress* fprogress = nullptr
        , TEnumAsByte<enum CalculateCollisionMethod> calCollision = CalculateCollisionMethod::eComplexCollision
        , bool CalTangentIfNone = false
        , TSubclassOf<AActor> actorClass = nullptr
        , UWorld* world = nullptr
    );

    UFUNCTION(BlueprintCallable, Category = "RuntimeLoadFbx|Async")
    static  URLFProgress* CreateProgress();

	UFUNCTION(BlueprintCallable, Category = "RuntimeLoadFbx|Utilit")
	static  void FocusActor(AActor* actor);
};
