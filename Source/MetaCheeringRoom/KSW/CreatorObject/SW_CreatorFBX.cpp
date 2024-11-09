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

	// 타이머핸들 제거
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void ASW_CreatorFBX::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 자식을 0,0,0으로 이동
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
	AActor* actor = fbxSubsystem->LoadFBX(FBXFileName);
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
