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

    // �⺻������ ���콺�� ����
	bShowMouseCursor = true;
    SetInputMode(FInputModeGameAndUI());

    // ���� ����
	CreatorWidget = CreateWidget<USW_CreatorWidget>(GetWorld(), CreatorWidgetFactory);
	CreatorWidget->AddToViewport();

	// PlayerController ��������
	CreatorWidget->ControllerReference = this;
}

void ASW_CreatorPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//for (int32 i = PendingDestroyActors.Num() - 1; i >= 0; --i)
	//{
	//	AActor* Actor = PendingDestroyActors[i];

	//	// Actor�� ��ȿ�ϰ� �ı� ���� �������� Ȯ��
	//	if (IsValid(Actor) && !Actor->IsPendingKillPending())
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("Destroying Actor: %s"), *Actor->GetName());
	//		Actor->Destroy();
	//		PendingDestroyActors.RemoveAt(i);  // �迭���� ����
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
        // ȭ�� ��ǥ�� ���� ��ǥ�� ��ȯ
        DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection);

        FVector TraceStart = WorldLocation;
        FVector TraceEnd = WorldLocation + (WorldDirection * 10000.0f);  // Ʈ���̽� �Ÿ� ����

        FHitResult HitResult;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(GetPawn());  // �ڽ��� ĳ���ʹ� ����

        // ���� Ʈ���̽� ����
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

        // ����׿� Ʈ���̽� ���� ǥ�� (������)
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
		// ���̸� ���� �ε����� �������� �̵���Ų��.
		FVector WorldLocation, WorldDirection;

		DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection);

		FVector TraceStart = WorldLocation;
		FVector TraceEnd = WorldLocation + (WorldDirection * 10000.0f);  // Ʈ���̽� �Ÿ� ����

		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(GetPawn());  // �ڽ��� ĳ���ʹ� ����

		Params.AddIgnoredActor(CreatingObject);  // �ڽ��� ĳ���ʹ� ����
		// ���� Ʈ���̽� ����
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params);

		if (bHit)
		{
			CreatingObject->SetActorLocation(HitResult.Location);
		}
		else
		{
			// ���̰� �ε����� �ʾ��� ��
			// ������ 500�Ÿ� �������� �̵�
			FVector p = GetPawn()->GetActorLocation();
			CreatingObject->SetActorLocation(p + WorldDirection * 500);
		}
	}
}

