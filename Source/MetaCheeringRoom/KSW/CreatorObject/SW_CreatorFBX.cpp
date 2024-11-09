// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorObject/SW_CreatorFBX.h"
#include "Net/UnrealNetwork.h"
#include "../CreatorFBXSubsystem.h"

ASW_CreatorFBX::ASW_CreatorFBX()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASW_CreatorFBX::BeginPlay()
{
 	Super::BeginPlay();
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
	// �ڽ��� 0,0,0���� �̵�
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (AActor* child : AttachedActors)
	{
		child->SetActorLocation(GetActorLocation());
	}
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
	AActor* actor = fbxSubsystem->LoadFBX(FBXFileName);
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
