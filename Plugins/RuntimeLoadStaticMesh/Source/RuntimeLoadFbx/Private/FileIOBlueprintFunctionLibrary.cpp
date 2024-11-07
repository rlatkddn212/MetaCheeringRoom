// Copyright 2021 ls Sun, Inc. All Rights Reserved.


#include "FileIOBlueprintFunctionLibrary.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "CommonImport.h"
#include "FileImporterAsync.h"
#include "FileImporterStaticMesh.h"


AActor* UFileIOBlueprintFunctionLibrary::LoadFileAsync2StaticMeshActor(const FString& filePath
	, URLFProgress* fprogress
	, TEnumAsByte<enum CalculateCollisionMethod> calCollision
	, bool CalTangentIfNone 
	, TSubclassOf<AActor> actorClass
	, UWorld* world)
{
    if (!world)world = GWorld;
	if (fprogress)fprogress->filePath = filePath;
	TWeakObjectPtr<AActor> weakActor = FileIO::LoadFileStaticMesh(
		filePath
		, true
		,fprogress
		, calCollision
		, CalTangentIfNone
		, actorClass
		, world
	);
    return weakActor.Get();
}

AActor* UFileIOBlueprintFunctionLibrary::LoadFileAsync2ProceduralMeshActor(const FString& filePath
    , URLFProgress* fprogress
	, TEnumAsByte<enum CalculateCollisionMethod> calCollision
	, bool CalTangentIfNone
	, TSubclassOf<AActor> actorClass
	, UWorld* world)
{
    if (!world)world = GWorld;
	if (fprogress)fprogress->filePath = filePath;
    TWeakObjectPtr<AActor> weakActor = FileIO::LoadFile2ProceduralMesh(
		filePath
		, true
		, fprogress
		, calCollision
		, CalTangentIfNone
		, actorClass
		, world);
    return weakActor.Get();
}

AActor* UFileIOBlueprintFunctionLibrary::LoadFileSync2StaticMeshActor(const FString& filePath
	, URLFProgress* fprogress /*= nullptr*/
	, TEnumAsByte<enum CalculateCollisionMethod> calCollision
	, bool CalTangentIfNone
	, TSubclassOf<AActor> actorClass
	, UWorld* world)
{
    if (!world)world = GWorld;
	if (fprogress)fprogress->filePath = filePath;
    TWeakObjectPtr<AActor> weakActor = FileIO::LoadFileStaticMesh(
		filePath
		,false
		, fprogress
		, calCollision
		, CalTangentIfNone
		, actorClass
		, world);
    return weakActor.Get();
}

AActor* UFileIOBlueprintFunctionLibrary::LoadFileSync2ProceduralMeshActor(const FString& filePath
	, URLFProgress* fprogress /*= nullptr*/
	, TEnumAsByte<enum CalculateCollisionMethod> calCollision
	, bool CalTangentIfNone
	, TSubclassOf<AActor> actorClass
	, UWorld* world)
{
	if (!world)world = GWorld;
	if (fprogress)fprogress->filePath = filePath;
	TWeakObjectPtr<AActor> weakActor = FileIO::LoadFile2ProceduralMesh(
		filePath
		, false
		, fprogress
		, calCollision
		, CalTangentIfNone
		, actorClass
		, world);
	return weakActor.Get();
}

URLFProgress* UFileIOBlueprintFunctionLibrary::CreateProgress()
{
	URLFProgress* fp = NewObject<URLFProgress>();
	fp->AddToRoot();
	return fp;
}

void UFileIOBlueprintFunctionLibrary::FocusActor(AActor* actor)
{
	if (!actor)return;
	APlayerController* pController = actor->GetWorld()->GetFirstPlayerController();
	if (!pController)return;
	APawn* pPawn = pController->GetPawn();
	if (!pPawn)return;

	FBox box = FileIO::GetActorBoundWithChildren(actor);
	if (!box.IsValid)return;
	FVector orig = box.GetCenter();
	float Radius = box.GetExtent().Size() * 2;
	const float HalfFOVRadians = FMath::DegreesToRadians(90.f / 2);

	FVector vForward = pController->PlayerCameraManager->GetActorForwardVector();// pPawn->GetActorForwardVector();
	const float DistanceFromSphere = Radius / FMath::Tan(HalfFOVRadians);
	FVector newOrig = orig - DistanceFromSphere * vForward;
	pPawn->SetActorLocation(newOrig);
}