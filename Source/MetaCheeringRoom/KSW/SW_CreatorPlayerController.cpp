// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/SW_CreatorPlayerController.h"
#include "KSW/CreatorObject/SW_CreatorObject.h"
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
#include "Widgets/InvalidateWidgetReason.h"
#include "Blueprint/UserWidget.h"
#include "Engine/TimerHandle.h"
#include "Creator/SW_CreatorGameState.h"

ASW_CreatorPlayerController::ASW_CreatorPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;

}

void ASW_CreatorPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
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
}

void ASW_CreatorPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASW_CreatorPlayerController::OnMouseOver()
{
	if (!SelectedObject)
		return;

	SelectedObject->SelectAxis(false, false, false);
	SelectedObject->SelectRotationAxis(false, false, false);
	SelectedObject->SelectScaleAxis(false, false, false);

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
					//UE_LOG(LogTemp, Log, TEXT("Clicked on Component: %s"), *HitResult.Component->GetName());

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
					else if (HitResult.Component->ComponentTags.Contains(FName("XYAxisMesh")))
					{
						SelectedObject->SelectAxis(true, true, false);
					}
					else if (HitResult.Component->ComponentTags.Contains(FName("XZAxisMesh")))
					{
						SelectedObject->SelectAxis(true, false, true);
					}
					else if (HitResult.Component->ComponentTags.Contains(FName("YZAxisMesh")))
					{
						SelectedObject->SelectAxis(false, true, true);
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
				}
			}
			else
			{
				// UE_LOG(LogTemp, Log, TEXT("No Actor hit."));
			}
		}
	}
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
					//UE_LOG(LogTemp, Log, TEXT("Clicked on Component: %s"), *HitResult.Component->GetName());

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
					else if (HitResult.Component->ComponentTags.Contains(FName("XYAxisMesh")))
					{
						SelectedObject->SelectAxis(true, true, false);
					}
					else if (HitResult.Component->ComponentTags.Contains(FName("XZAxisMesh")))
					{
						SelectedObject->SelectAxis(true, false, true);
					}
					else if (HitResult.Component->ComponentTags.Contains(FName("YZAxisMesh")))
					{
						SelectedObject->SelectAxis(false, true, true);
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
				//UE_LOG(LogTemp, Log, TEXT("No Actor hit."));
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
				else
				{
					UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
					// �θ� ASW_CreatorObject���� Ȯ��
					ASW_CreatorObject* ParentObject = system->GetParentCreatorObject(HitResult.GetActor());
					if (ParentObject)
					{
						DoSelectObject(ParentObject);
					}
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

// �Ⱦ�
void ASW_CreatorPlayerController::CreatingDummyObject(struct FCreatorObjectData* ObjectData)
{
	CreatingObject = nullptr;
	UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
	CreatingObject = system->CreateDummyObject(ObjectData);

	DoSelectObject(nullptr);
	CreatorWidget->OnDragged(true);
}

void ASW_CreatorPlayerController::ImportFBXObject(const FString& FilePath)
{
	// server �۾�
	int32 CreatorObjectType = 5;
	int32 CreatorObjectId = 1; 
	UCreatorStorageSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorStorageSubsystem>();

	TMap<int32, FCreatorObjectData*> CreatorObjectsStruct = system->GetCreatorObjects(CreatorObjectType);
	if (CreatorObjectsStruct.Contains(CreatorObjectId))
	{
		// CreatorObject�� ����
		// CreatorObjectsStruct[CreatorObjectId]

		// �����ؼ� ����
		FCreatorObjectData* CreatorObjectData = new FCreatorObjectData(*CreatorObjectsStruct[CreatorObjectId]);

		CreatorObjectData->ItemName = FilePath;

		Server_AddCreatingDummyObject(*CreatorObjectData, FVector::ZeroVector);
		// OpenAndCopyFBX(SelectedFilePath, FileName, ProgressTracker);
	}
}

void ASW_CreatorPlayerController::Server_SetOwnerObject_Implementation(class ASW_CreatorObject* OwnerObject, bool isOnwer)
{
	if (OwnerObject == nullptr)
		return;
	if (isOnwer)
	{
		OwnerObject->SetOwner(this);
	}
	else
	{
		OwnerObject->SetOwner(nullptr);
	}
}

void ASW_CreatorPlayerController::Server_AddCreatingDummyObject_Implementation(const struct FCreatorObjectData& ObjectData, FVector Pos)
{
	UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
	ASW_CreatorObject* NewCreatingObject = system->CreateObject(&ObjectData);
	// ��Ƽ
	if (NewCreatingObject)
	{
		NewCreatingObject->SetActorLocation(Pos);
		// Timer
		// PC�� ������
		NewCreatingObject->SetOwner(this);
		NewCreatingObject->SetFileName(ObjectData.ItemName);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, NewCreatingObject, ObjectData]()
			{
				// ���ӽ�����Ʈ
				ASW_CreatorGameState* GameState = GetWorld()->GetGameState<ASW_CreatorGameState>();
				if (GameState)
				{
					GameState->Multicast_AddCreatingDummyObject(NewCreatingObject, ObjectData);
				}
			}, 0.1f, false);

		// ���ӽ�����Ʈ
		/*ASW_CreatorGameState* GameState = GetWorld()->GetGameState<ASW_CreatorGameState>();
		if (GameState)
		{
			GameState->Multicast_AddCreatingDummyObject(NewCreatingObject);
		}*/
	}
}

void ASW_CreatorPlayerController::Server_DeleteObject_Implementation(class ASW_CreatorObject* DeleteObject)
{
	// ���ӽ�����Ʈ
	ASW_CreatorGameState* GameState = GetWorld()->GetGameState<ASW_CreatorGameState>();
	if (GameState)
	{
		GameState->Multicast_DeleteObject(DeleteObject);
	}
}

void ASW_CreatorPlayerController::DoSelectObject(class ASW_CreatorObject* NewSelectObject)
{
	if (SelectedObject)
	{
		SelectedObject->OnSelected(false);
		Server_SetOwnerObject(SelectedObject, false);
	}

	SelectedObject = NewSelectObject;
	
	CreatorWidget->CreatorInspectorWidget->SetObject(SelectedObject);
	if (SelectedObject)
	{
		SelectedObject->OnSelected(true);
		Server_SetOwnerObject(SelectedObject, true);
		SelectedObject->ChangeToolMode(ToolState);
	}
}

void ASW_CreatorPlayerController::UnSelectServerDeleteObject(class ASW_CreatorObject* ServerDeleteObject)
{
	if (SelectedObject == ServerDeleteObject)
	{
		//if (SelectedObject) SelectedObject->OnSelected(false);
		Server_SetOwnerObject(SelectedObject, false);
		SelectedObject = nullptr;
		CreatorWidget->CreatorInspectorWidget->SetObject(nullptr);
	}
}

bool ASW_CreatorPlayerController::EndDragDummyObject()
{
	bool ret = CreatorWidget->IsDragOverUI();
	if (!ret)
	{
		if (CreatingObject)
		{
			Server_AddCreatingDummyObject(*CreatingObject->CreatingObjectData, CreatingObject->GetActorLocation());
			UCreatorStorageSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorStorageSubsystem>();
			system->AddLRUCreatorObject(CreatingObject->CreatingObjectData);
		}
	}

	if (CreatingObject)
	{
		CreatingObject->Destroy();
		CreatingObject = nullptr;
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
		Server_DeleteObject(SelectedObject);
		CreatorWidget->CreatorInspectorWidget->SetObject(nullptr);
		SelectedObject = nullptr;
	}
}

void ASW_CreatorPlayerController::ReloadHierarchy()
{
	CreatorWidget->CreatorHierarchyWidget->ReloadItem();
}

void ASW_CreatorPlayerController::CopySelectedObject()
{
	CopiedObject = SelectedObject;
}

void ASW_CreatorPlayerController::PasteSelectedObject()
{
	if (CopiedObject)
	{
		UCreatorMapSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();
		ASW_CreatorObject* NewSelectedObject = system->CopyObject(CopiedObject);
		DoSelectObject(NewSelectedObject);

		ReloadHierarchy();
	}
}

