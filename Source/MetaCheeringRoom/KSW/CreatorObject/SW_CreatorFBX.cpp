// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorObject/SW_CreatorFBX.h"
#include "Net/UnrealNetwork.h"
#include "../CreatorFBXSubsystem.h"
#include "RLFProgress.h"
#include "../UI/SW_FBXLoadProgressWidget.h"
#include "Components/WidgetComponent.h"

ASW_CreatorFBX::ASW_CreatorFBX()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	LoadProgressWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("LoadProgressWidgetComponent"));
	LoadProgressWidgetComponent->SetupAttachment(Root);
}

void ASW_CreatorFBX::BeginPlay()
{
 	Super::BeginPlay();

	ProgressTracker = NewObject<URLFProgress>();
	ProgressTracker->progressCal.AddDynamic(this, &ThisClass::HandleProgress);
	ProgressTracker->finishCal.AddDynamic(this, &ThisClass::HandleFinish);

	// 위젯을 생성한다.
	LoadProgressWidget = Cast<USW_FBXLoadProgressWidget>(LoadProgressWidgetComponent->GetUserWidgetObject());
	if (LoadProgressWidget)
	{
		LoadProgressWidget->SetVisibility(ESlateVisibility::Visible);
		LoadProgressWidget->SetBar(0.0f);
	}
}

void ASW_CreatorFBX::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// 타이머핸들 제거
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void ASW_CreatorFBX::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 위젯을 카메라 방향으로 돌린다.
	if (LoadProgressWidgetComponent)
	{
		FVector Target = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
		FVector Dir = Target - LoadProgressWidgetComponent->GetComponentLocation();
		FRotator Rot = Dir.ToOrientationRotator();
		LoadProgressWidgetComponent->SetWorldRotation(Rot);
	}

	// 자식을 0,0,0으로 이동
	//TArray<AActor*> AttachedActors;
	//GetAttachedActors(AttachedActors);
	//for (AActor* child : AttachedActors)
	//{
	//	child->SetActorLocation(GetActorLocation());
	//}
}

void ASW_CreatorFBX::OnSelected(bool isSelected)
{
	Super::OnSelected(isSelected);
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	// Attach된 자식 액터를 확인한다.
	for (AActor* child : AttachedActors)
	{
		// 자식 액터의 Root가 Static Mesh라면 선택 상태 변경
		SelectChildActor(child, isSelected);
	}
}

void ASW_CreatorFBX::SelectChildActor(AActor* actor, bool isSelected)
{
	if (actor->GetRootComponent()->IsA(UStaticMeshComponent::StaticClass()))
	{
		UStaticMeshComponent* comp = Cast<UStaticMeshComponent>(actor->GetRootComponent());
		comp->SetRenderCustomDepth(isSelected);
	}

	TArray<AActor*> AttachedActors;
	actor->GetAttachedActors(AttachedActors);
	for (AActor* child : AttachedActors)
	{
		SelectChildActor(child, isSelected);
	}
}

void ASW_CreatorFBX::SetFileName(const FString& FileName)
{
	// 이미 파일을 로드한 경우에는 다시 로드하지 않는다.
	if (FBXFileName == FileName)
	{
		return;
	}

	FBXFileName = FileName; 
	if (HasAuthority())
	{
		//한프레임 쉰다.
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASW_CreatorFBX::OnRep_FBXFileName, 0.3f, false);
	}
}

void ASW_CreatorFBX::RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const
{
	Super::RecordJsonAdditionalInfo(RecordJsonObject);

	// 추가 정보를 기록한다.
	RecordJsonObject->SetStringField(TEXT("FBXFileName"), FBXFileName);
}

void ASW_CreatorFBX::SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject)
{
	Super::SetupJsonAdditionalInfo(SetupJsonObject);

	// 추가 정보를 설정한다.
	FBXFileName = SetupJsonObject->GetStringField(TEXT("FBXFileName"));
	if (HasAuthority())
	{
		//한프레임 쉰다.
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASW_CreatorFBX::OnRep_FBXFileName, 0.3f, false);
	}
}

void ASW_CreatorFBX::OnRep_FBXFileName()
{
	// Replicate된 FBX 파일 이름이 변경되었을 때 호출된다.
	UCreatorFBXSubsystem* fbxSubsystem = GetGameInstance()->GetSubsystem<UCreatorFBXSubsystem>();
	if (! fbxSubsystem->IsFileExist(FBXFileName))
	{
		fbxSubsystem->FileDownloadFromFirebase(FPaths::ProjectSavedDir() + TEXT("FBX/"), FBXFileName, [this]() {
			// 0.5초후 실행
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASW_CreatorFBX::OpenFBXFile, 0.5f, false);
			});
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASW_CreatorFBX::OpenFBXFile, 0.5f, false);
		//OpenFBXFile();
	}
}

void ASW_CreatorFBX::OpenFBXFile()
{
	UCreatorFBXSubsystem* fbxSubsystem = GetGameInstance()->GetSubsystem<UCreatorFBXSubsystem>();
	if (!fbxSubsystem->IsFileExist(FBXFileName))
	{
		UE_LOG(LogTemp, Error, TEXT("File not found: %s"), *FBXFileName);
		return;
	}

	AActor* actor = fbxSubsystem->LoadFBX(FBXFileName, ProgressTracker);
	// 액터 위치를 CreatorFBX의 위치로 설정한다.
	if (actor)
	{
		actor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void ASW_CreatorFBX::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 추가 정보를 Replicate한다.
	DOREPLIFETIME(ASW_CreatorFBX, FBXFileName);
}

void ASW_CreatorFBX::HandleProgress(const FString& FilePath, float Progress)
{
	UE_LOG(LogTemp, Log, TEXT("File %s is %.2f%% loaded."), *FilePath, Progress * 100);
	// 위젯에 진행률을 표시한다.
	if (LoadProgressWidget)
	{
		LoadProgressWidget->SetBar(Progress);
	}
}

void ASW_CreatorFBX::HandleFinish(const FString& FilePath, AActor* ImportedActor)
{
	UE_LOG(LogTemp, Log, TEXT("File %s is loaded."), *FilePath);
	// 액터 위치를 CreatorFBX의 위치로 설정한다.
	if (ImportedActor)
	{
		// Attach
		ImportedActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		ImportedActor->SetActorLocation(GetActorLocation());

		// 메쉬를 숨긴다.
		Mesh->SetVisibility(false);
		// 위젯을 숨긴다.
		if (LoadProgressWidget)
		{
			LoadProgressWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
