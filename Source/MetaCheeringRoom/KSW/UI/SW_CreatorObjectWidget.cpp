// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_CreatorObjectWidget.h"
#include "SW_CreatorObjectSlotWidget.h"
#include "../CreatorStorageSubsystem.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "IDesktopPlatform.h"
#include "DesktopPlatformModule.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SWidget.h"
#include "../../../../Plugins/RuntimeLoadStaticMesh/Source/RuntimeLoadFbx/Public/FileIOBlueprintFunctionLibrary.h"
#include "../CreatorMapSubsystem.h"
#include "../CreatorFBXSubsystem.h"

void USW_CreatorObjectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UCreatorStorageSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorStorageSubsystem>();
	{
		TMap<int32, FCreatorObjectData*> CreatorObjects = system->GetCreatorObjects(1);
		for (auto& CreatorObject : CreatorObjects)
		{
			USW_CreatorObjectSlotWidget* ChildWidget = CreateWidget<USW_CreatorObjectSlotWidget>(GetWorld(), SlotFactory);
			ChildWidget->SetObject(CreatorObject.Value);
			ShapeObjectScrollBox->AddChild(ChildWidget);
		}
	}
	{
		TMap<int32, FCreatorObjectData*> CreatorObjects = system->GetCreatorObjects(2);
		for (auto& CreatorObject : CreatorObjects)
		{
			USW_CreatorObjectSlotWidget* ChildWidget = CreateWidget<USW_CreatorObjectSlotWidget>(GetWorld(), SlotFactory);
			ChildWidget->SetObject(CreatorObject.Value);
			SportsObjectScrollBox->AddChild(ChildWidget);
		}
	}
	
	{
		TMap<int32, FCreatorObjectData*> CreatorObjects = system->GetCreatorObjects(3);
		for (auto& CreatorObject : CreatorObjects)
		{
			USW_CreatorObjectSlotWidget* ChildWidget = CreateWidget<USW_CreatorObjectSlotWidget>(GetWorld(), SlotFactory);
			ChildWidget->SetObject(CreatorObject.Value);
			LightObjectScrollBox->AddChild(ChildWidget);
		}
	}

	{
		TMap<int32, FCreatorObjectData*> CreatorObjects = system->GetCreatorObjects(4);
		for (auto& CreatorObject : CreatorObjects)
		{
			USW_CreatorObjectSlotWidget* ChildWidget = CreateWidget<USW_CreatorObjectSlotWidget>(GetWorld(), SlotFactory);
			ChildWidget->SetObject(CreatorObject.Value);
			EffectObjectScrollBox->AddChild(ChildWidget);
		}
	}
	// 버튼 클릭처리
	ShapeObjectButton->OnClicked.AddDynamic(this, &USW_CreatorObjectWidget::OnShapeObjectButtonClicked);
	SportsObjectButton->OnClicked.AddDynamic(this, &USW_CreatorObjectWidget::OnSportsObjectButtonClicked);
	LightObjectButton->OnClicked.AddDynamic(this, &USW_CreatorObjectWidget::OnLightObjectButtonClicked);
	EffectObjectButton->OnClicked.AddDynamic(this, &USW_CreatorObjectWidget::OnEffectObjectButtonClicked);
	FBXObjectButton->OnClicked.AddDynamic(this, &USW_CreatorObjectWidget::OnFBXObjectButtonClicked);

	ImportButton->OnClicked.AddDynamic(this, &USW_CreatorObjectWidget::OnImportButtonClicked);

	SetCreatorObjectTabState(ECreatorObjectTabState::ShapeObject);
}

void USW_CreatorObjectWidget::OnImportButtonClicked()
{
	// DesktopPlatform 인스턴스 가져오기
	if (IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get())
	{
		// 파일 필터 설정
		FString FileTypes = TEXT("All Files (*.*)|*.*|Texture Files (*.png;*.jpg)|*.png;*.jpg");

		// 파일 경로를 저장할 배열
		TArray<FString> OutFileNames;

		// 부모 윈도우 핸들 가져오기
		const void* ParentWindowHandle = FSlateApplication::Get().GetActiveTopLevelWindow()->GetNativeWindow()->GetOSWindowHandle();

		// 파일 선택 다이얼로그 열기
		const bool bOpened = DesktopPlatform->OpenFileDialog(
			ParentWindowHandle,
			TEXT("파일 선택"),
			FPaths::ProjectContentDir(),
			TEXT(""),
			FileTypes,
			EFileDialogFlags::None,
			OutFileNames
		);

		// 파일이 선택되었다면
		if (bOpened && OutFileNames.Num() > 0)
		{
			// 선택된 파일 경로 
			FString SelectedFilePath = OutFileNames[0];

			// 여기서 선택된 파일로 작업 수행
			UE_LOG(LogTemp, Log, TEXT("Selected File: %s"), *SelectedFilePath);

			// FBX 파일 로드
			// LoadFileAsync2StaticMeshActor(SelectedFilePath);

			UCreatorFBXSubsystem* fbxSubsystem = GetGameInstance()->GetSubsystem<UCreatorFBXSubsystem>();
			AActor* actor = fbxSubsystem->OpenFBX(SelectedFilePath);

			UCreatorStorageSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorStorageSubsystem>();
			UCreatorMapSubsystem* CreatorMapSubsystem = GetGameInstance()->GetSubsystem<UCreatorMapSubsystem>();

			int32 CreatorObjectType = 5;
			int32 CreatorObjectId = 1;
			TMap<int32, FCreatorObjectData*> CreatorObjectsStruct = system->GetCreatorObjects(CreatorObjectType);
			if (CreatorObjectsStruct.Contains(CreatorObjectId))
			{
				// CreatorObject를 생성
				ASW_CreatorObject* CreatorObject = CreatorMapSubsystem->CreateObject(CreatorObjectsStruct[CreatorObjectId]);

				actor->AttachToActor(CreatorObject, FAttachmentTransformRules::KeepWorldTransform);

				CreatorMapSubsystem->AddObject(CreatorObject);
			}

			ASW_CreatorPlayerController* PC = Cast<ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());
			if (PC)
				PC->ReloadHierarchy();
		}
	}
}

void USW_CreatorObjectWidget::OnShapeObjectButtonClicked()
{
	SetCreatorObjectTabState(ECreatorObjectTabState::ShapeObject);
}

void USW_CreatorObjectWidget::OnSportsObjectButtonClicked()
{
	SetCreatorObjectTabState(ECreatorObjectTabState::SportsObject);
}

void USW_CreatorObjectWidget::OnLightObjectButtonClicked()
{
	SetCreatorObjectTabState(ECreatorObjectTabState::LightObject);
}

void USW_CreatorObjectWidget::OnEffectObjectButtonClicked()
{
	SetCreatorObjectTabState(ECreatorObjectTabState::EffectObject);
}

void USW_CreatorObjectWidget::OnFBXObjectButtonClicked()
{
	SetCreatorObjectTabState(ECreatorObjectTabState::FBXObject);
}

void USW_CreatorObjectWidget::SetCreatorObjectTabState(ECreatorObjectTabState state)
{
	ShapeObjectButton->SetStyle(DefaultButtonStyle);
	SportsObjectButton->SetStyle(DefaultButtonStyle);
	LightObjectButton->SetStyle(DefaultButtonStyle);
	EffectObjectButton->SetStyle(DefaultButtonStyle);
	FBXObjectButton->SetStyle(DefaultButtonStyle);
	
	ShapeBorder->SetVisibility(ESlateVisibility::Hidden);
	SportsBorder->SetVisibility(ESlateVisibility::Hidden);
	LightBorder->SetVisibility(ESlateVisibility::Hidden);
	EffectBorder->SetVisibility(ESlateVisibility::Hidden);
	FBXBorder->SetVisibility(ESlateVisibility::Hidden);

	if (state == ECreatorObjectTabState::ShapeObject)
	{
		ShapeObjectButton->SetStyle(EnableButtonStyle);
		ShapeBorder->SetVisibility(ESlateVisibility::Visible);
	}
	else if (state == ECreatorObjectTabState::SportsObject)
	{
		SportsObjectButton->SetStyle(EnableButtonStyle);
		SportsBorder->SetVisibility(ESlateVisibility::Visible);
	}
	else if (state == ECreatorObjectTabState::LightObject)
	{
		LightObjectButton->SetStyle(EnableButtonStyle);
		LightBorder->SetVisibility(ESlateVisibility::Visible);
	}
	else if (state == ECreatorObjectTabState::EffectObject)
	{
		EffectObjectButton->SetStyle(EnableButtonStyle);
		EffectBorder->SetVisibility(ESlateVisibility::Visible);
	}
	else if (state == ECreatorObjectTabState::FBXObject)
	{
		FBXObjectButton->SetStyle(EnableButtonStyle);
		FBXBorder->SetVisibility(ESlateVisibility::Visible);
	}
}
