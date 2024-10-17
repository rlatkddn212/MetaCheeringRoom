// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/SW_CreatorPlayerController.h"
#include "SW_CreatorObject.h"
#include "CreatorStorageSubsystem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "UI/SW_CreatorWidget.h"
#include "Layout/Geometry.h"
#include "UObject/NoExportTypes.h"
#include "Layout/SlateRotatedRect.h"
#include "Types/SlateVector2.h"
#include "CreatorMapSubsystem.h"
#include "UObject/FastReferenceCollector.h"
#include "Engine/Engine.h"

ASW_CreatorPlayerController::ASW_CreatorPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;

}

void ASW_CreatorPlayerController::BeginPlay()
{
	Super::BeginPlay();

    // 기본적으로 마우스가 보임
	bShowMouseCursor = true;
    SetInputMode(FInputModeGameAndUI());

    // 위젯 생성
	CreatorWidget = CreateWidget<USW_CreatorWidget>(GetWorld(), CreatorWidgetFactory);
	CreatorWidget->AddToViewport();

	// PlayerController 가져오기
	CreatorWidget->ControllerReference = this;
}

void ASW_CreatorPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//for (int32 i = PendingDestroyActors.Num() - 1; i >= 0; --i)
	//{
	//	AActor* Actor = PendingDestroyActors[i];

	//	// Actor가 유효하고 파괴 가능 상태인지 확인
	//	if (IsValid(Actor) && !Actor->IsPendingKillPending())
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("Destroying Actor: %s"), *Actor->GetName());
	//		Actor->Destroy();
	//		PendingDestroyActors.RemoveAt(i);  // 배열에서 제거
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("Actor not ready for destruction: %s"), *Actor->GetName());
	//	}
	//}
}

void ASW_CreatorPlayerController::OnLeftClick()
{
    FVector2D MousePosition;
    if (GetMousePosition(MousePosition.X, MousePosition.Y))
    {
        FVector WorldLocation, WorldDirection;
        // 화면 좌표를 월드 좌표로 변환
        DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection);

        FVector TraceStart = WorldLocation;
        FVector TraceEnd = WorldLocation + (WorldDirection * 10000.0f);  // 트레이스 거리 설정

        FHitResult HitResult;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(GetPawn());  // 자신의 캐릭터는 무시

        // 라인 트레이스 실행
        bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params);

        if (bHit)
        {
            UE_LOG(LogTemp, Log, TEXT("Clicked on Actor: %s"), *HitResult.GetActor()->GetName());
            ASW_CreatorObject* Object = Cast<ASW_CreatorObject>(HitResult.GetActor());
            if (Object)
            {
                Object->OnSelected(true);
				SelectedObject->OnSelected(false);
				SelectedObject = Object;
            }
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("No Actor hit."));
        }

        // 디버그용 트레이스 라인 표시 (빨간색)
        DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f);
    }
}

void ASW_CreatorPlayerController::CreatingDummyObject(struct FCreatorObjectData* ObjectData)
{
	FActorSpawnParameters SpawnParams;
	//SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CreatingObject = GetWorld()->SpawnActor<ASW_CreatorObject>(ObjectData->ItemClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	CreatingObject->SetActorLocation(FVector::ZeroVector);
	CreatingObject->SetActorRotation(FRotator::ZeroRotator);

	CreatorWidget->OnDragged(true);
}

bool ASW_CreatorPlayerController::DeleteDummyObject()
{
	bool ret = CreatorWidget->IsDragOverUI();
	if (ret)
	{
		if (CreatingObject)
		{
			CreatingObject->ConditionalBeginDestroy();
		}
	}

	CreatorWidget->OnDragged(false);

	return ret;
}

void ASW_CreatorPlayerController::MoveDummyObject(FVector2D MousePosition)
{
	bool ret = CreatorWidget->IsDragOverUI();
	if (CreatingObject)
	{
		CreatingObject->SetActorHiddenInGame(ret);
	}

	if (CreatingObject)
	{
		// 레이를 쏴서 부딪히는 지점으로 이동시킨다.
		FVector WorldLocation, WorldDirection;

		DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection);

		FVector TraceStart = WorldLocation;
		FVector TraceEnd = WorldLocation + (WorldDirection * 10000.0f);  // 트레이스 거리 설정

		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(GetPawn());  // 자신의 캐릭터는 무시

		Params.AddIgnoredActor(CreatingObject);  // 자신의 캐릭터는 무시
		// 라인 트레이스 실행
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params);

		if (bHit)
		{
			CreatingObject->SetActorLocation(HitResult.Location);
		}
		else
		{
			// 레이가 부딪히지 않았을 때
			// 레이의 500거리 지점으로 이동
			FVector p = GetPawn()->GetActorLocation();
			CreatingObject->SetActorLocation(p + WorldDirection * 500);
		}
	}
}

