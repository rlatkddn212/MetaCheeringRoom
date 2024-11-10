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

	// ������ �����Ѵ�.
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

	// Ÿ�̸��ڵ� ����
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void ASW_CreatorFBX::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// ������ ī�޶� �������� ������.
	if (LoadProgressWidgetComponent)
	{
		FVector Target = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
		FVector Dir = Target - LoadProgressWidgetComponent->GetComponentLocation();
		FRotator Rot = Dir.ToOrientationRotator();
		LoadProgressWidgetComponent->SetWorldRotation(Rot);
	}

	// �ڽ��� 0,0,0���� �̵�
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
	// Attach�� �ڽ� ���͸� Ȯ���Ѵ�.
	for (AActor* child : AttachedActors)
	{
		// �ڽ� ������ Root�� Static Mesh��� ���� ���� ����
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
	// �̹� ������ �ε��� ��쿡�� �ٽ� �ε����� �ʴ´�.
	if (FBXFileName == FileName)
	{
		return;
	}

	FBXFileName = FileName; 
	if (HasAuthority())
	{
		//�������� ����.
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASW_CreatorFBX::OnRep_FBXFileName, 0.3f, false);
	}
}

void ASW_CreatorFBX::RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const
{
	Super::RecordJsonAdditionalInfo(RecordJsonObject);

	// �߰� ������ ����Ѵ�.
	RecordJsonObject->SetStringField(TEXT("FBXFileName"), FBXFileName);
}

void ASW_CreatorFBX::SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject)
{
	Super::SetupJsonAdditionalInfo(SetupJsonObject);

	// �߰� ������ �����Ѵ�.
	FBXFileName = SetupJsonObject->GetStringField(TEXT("FBXFileName"));
	if (HasAuthority())
	{
		//�������� ����.
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASW_CreatorFBX::OnRep_FBXFileName, 0.3f, false);
	}
}

void ASW_CreatorFBX::OnRep_FBXFileName()
{
	// Replicate�� FBX ���� �̸��� ����Ǿ��� �� ȣ��ȴ�.
	UCreatorFBXSubsystem* fbxSubsystem = GetGameInstance()->GetSubsystem<UCreatorFBXSubsystem>();
	if (! fbxSubsystem->IsFileExist(FBXFileName))
	{
		fbxSubsystem->FileDownloadFromFirebase(FPaths::ProjectSavedDir() + TEXT("FBX/"), FBXFileName, [this]() {
			// 0.5���� ����
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
	// ���� ��ġ�� CreatorFBX�� ��ġ�� �����Ѵ�.
	if (actor)
	{
		actor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void ASW_CreatorFBX::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// �߰� ������ Replicate�Ѵ�.
	DOREPLIFETIME(ASW_CreatorFBX, FBXFileName);
}

void ASW_CreatorFBX::HandleProgress(const FString& FilePath, float Progress)
{
	UE_LOG(LogTemp, Log, TEXT("File %s is %.2f%% loaded."), *FilePath, Progress * 100);
	// ������ ������� ǥ���Ѵ�.
	if (LoadProgressWidget)
	{
		LoadProgressWidget->SetBar(Progress);
	}
}

void ASW_CreatorFBX::HandleFinish(const FString& FilePath, AActor* ImportedActor)
{
	UE_LOG(LogTemp, Log, TEXT("File %s is loaded."), *FilePath);
	// ���� ��ġ�� CreatorFBX�� ��ġ�� �����Ѵ�.
	if (ImportedActor)
	{
		// Attach
		ImportedActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		ImportedActor->SetActorLocation(GetActorLocation());

		// �޽��� �����.
		Mesh->SetVisibility(false);
		// ������ �����.
		if (LoadProgressWidget)
		{
			LoadProgressWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
