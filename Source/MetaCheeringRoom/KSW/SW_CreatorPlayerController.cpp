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
#include "UI/SW_CreatorHierarchyWidget.h"
#include "UI/SW_CreatorModelToolbarWidget.h"
#include "UI/SW_CreatorInspectorWidget.h"

ASW_CreatorPlayerController::ASW_CreatorPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;

}

void ASW_CreatorPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	SetInputMode(FInputModeGameAndUI());

    // ���� ����
	CreatorWidget = CreateWidget<USW_CreatorWidget>(GetWorld(), CreatorWidgetFactory);
	CreatorWidget->AddToViewport();

	// PlayerController ��������
	CreatorWidget->ControllerReference = this;
	CreatorWidget->ModelToolbarWidget->SetCreatorToolState(ECreatorToolState::Selection);

	CreatorWidget->ModelToolbarWidget->OnCreatorToolStateChanged.BindLambda([this](ECreatorToolState NewState)
		{
			SetToolState(NewState);
		});


	ReloadHierarchy();
	ToolState = ECreatorToolState::Selection;
}

void ASW_CreatorPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

bool ASW_CreatorPlayerController::OnLeftClick()
{
    FVector2D MousePosition;
    if (GetMousePosition(MousePosition.X, MousePosition.Y))
    {
        FVector WorldLocation, WorldDirection;
        // ȭ�� ��ǥ�� ���� ��ǥ�� ��ȯ
        DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection);
		
		{
			FVector TraceStart = WorldLocation;
			FVector TraceEnd = WorldLocation + (WorldDirection * 10000.0f);  // Ʈ���̽� �Ÿ� ����

			FHitResult HitResult;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(GetPawn());  // �ڽ��� ĳ���ʹ� ����

			FCollisionObjectQueryParams ObjectParams;
			ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel3);
			
			// ���� Ʈ���̽� ����
			bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, TraceStart, TraceEnd, ObjectParams, Params);

			if (bHit)
			{
				if (bHit && HitResult.Component.IsValid())
				{
					// �ƴ� ������Ʈ �̸� ���
					UE_LOG(LogTemp, Log, TEXT("Clicked on Component: %s"), *HitResult.Component->GetName());

					if (HitResult.Component->ComponentTags.Contains(FName("XAxisMesh")))
					{
						SelectedObject->SelectAxis(true, false, false);
					}
					else if (HitResult.Component->ComponentTags.Contains(FName("YAxisMesh")))
					{
						SelectedObject->SelectAxis(false, true, false);
					}
					else if (HitResult.Component->ComponentTags.Contains(FName("ZAxisMesh")))
					{
						SelectedObject->SelectAxis(false, false, true);
					}
					else if (HitResult.Component->ComponentTags.Contains(FName("XRingMesh")))
					{
						SelectedObject->SelectRotationAxis(true, false, false);
					}
					else if (HitResult.Component->ComponentTags.Contains(FName("YRingMesh")))
					{
						SelectedObject->SelectRotationAxis(false, true, false);
					}
					else if (HitResult.Component->ComponentTags.Contains(FName("ZRingMesh")))
					{
						SelectedObject->SelectRotationAxis(false, false, true);
					}
					else if (HitResult.Component->ComponentTags.Contains(FName("XScaleMesh")))
					{
						SelectedObject->SelectScaleAxis(true, false, false);
					}
					else if (HitResult.Component->ComponentTags.Contains(FName("YScaleMesh")))
					{
						SelectedObject->SelectScaleAxis(false, true, false);
					}
					else if (HitResult.Component->ComponentTags.Contains(FName("ZScaleMesh")))
					{
						SelectedObject->SelectScaleAxis(false, false, true);
					}

					return true;
				}
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("No Actor hit."));
			}
		}

		{
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
					DoSelectObject(Object);
				}
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("No Actor hit."));
			}
		}
        

        // ����׿� Ʈ���̽� ���� ǥ�� (������)
        //DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f);
    }

	return false;
}

void ASW_CreatorPlayerController::CreatingDummyObject(struct FCreatorObjectData* ObjectData)
{
	UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
	CreatingObject = system->CreateObject(ObjectData);

	DoSelectObject(CreatingObject);
	
	CreatorWidget->OnDragged(true);
}

void ASW_CreatorPlayerController::DoSelectObject(class ASW_CreatorObject* NewSelectObject)
{
	if (SelectedObject)
		SelectedObject->OnSelected(false);
	NewSelectObject->OnSelected(true);
	SelectedObject = NewSelectObject;

	CreatorWidget->CreatorInspectorWidget->SetObject(SelectedObject);
	if (SelectedObject)
		SelectedObject->ChangeToolMode(ToolState);
}

bool ASW_CreatorPlayerController::DeleteDummyObject()
{
	bool ret = CreatorWidget->IsDragOverUI();
	if (ret)
	{
		if (CreatingObject)
		{
			CreatingObject->Destroy();
			CreatingObject = nullptr;
		}
	}
	else
	{
		if (CreatingObject)
		{
			UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
			system->AddObject(CreatingObject);
			ReloadHierarchy();
			OnObjectChanged();
			DoSelectObject(CreatingObject);
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
			// ������ 1000�Ÿ� �������� �̵�
			FVector p = GetPawn()->GetActorLocation();
			CreatingObject->SetActorLocation(p + WorldDirection * 1000);
		}
	}
}

void ASW_CreatorPlayerController::SetToolState(ECreatorToolState NewState)
{
	ToolState = NewState;
	if (CreatorWidget)
		CreatorWidget->ModelToolbarWidget->SetCreatorToolState(NewState);
	
	if (SelectedObject)
		SelectedObject->ChangeToolMode(ToolState);
}

void ASW_CreatorPlayerController::Drag(FVector2D MouseDownPosition)
{
	FVector2D MousePosition;
	if (GetMousePosition(MousePosition.X, MousePosition.Y))
	{
		// �ʱⰪ
		if (SelectedObject)
		{
			SelectedObject->Drag(MouseDownPosition, MousePosition);

			// ���콺 ��ġ �α�
			UE_LOG(LogTemp, Log, TEXT("Mouse Position: %s"), *MousePosition.ToString());
		}
	}
}

void ASW_CreatorPlayerController::DragEnd()
{
	if (SelectedObject)
	{
		SelectedObject->DragEnd(ToolState);
	}
}

void ASW_CreatorPlayerController::OnObjectChanged()
{
	if (SelectedObject)
	{
		CreatorWidget->CreatorInspectorWidget->OnChanged();
	}
}

void ASW_CreatorPlayerController::DeleteSelectedObject()
{
	if (SelectedObject)
	{
		CreatorWidget->CreatorInspectorWidget->SetObject(nullptr);
		UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
		system->RemoveObject(SelectedObject, true);
		SelectedObject = nullptr;

		ReloadHierarchy();
	}
}

void ASW_CreatorPlayerController::ReloadHierarchy()
{
	CreatorWidget->CreatorHierarchyWidget->ReloadItem();
}

