// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorObject/SW_CreatorFBX.h"

ASW_CreatorFBX::ASW_CreatorFBX()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASW_CreatorFBX::BeginPlay()
{
 	Super::BeginPlay();
}

void ASW_CreatorFBX::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASW_CreatorFBX::OnSelected(bool isSelected)
{
	Super::OnSelected(isSelected);

	// Attach된 자식 액터를 확인한다.
	for (AActor* child : Children)
	{
		// 자식 액터의 Root가 Static Mesh라면 선택 상태 변경
		{
			SelectChildActor(child, isSelected);
		}
	}
}

void ASW_CreatorFBX::SelectChildActor(AActor* actor, bool isSelected)
{
	if (actor->GetRootComponent()->IsA(UStaticMeshComponent::StaticClass()))
	{
		UStaticMeshComponent* sceneComponent = Cast<UStaticMeshComponent>(actor->GetRootComponent());
		if (sceneComponent->GetAttachParent() == Root)
		{
			sceneComponent->SetRenderCustomDepth(isSelected);
		}
	}

	for (AActor* child : actor->Children)
	{
		SelectChildActor(child, isSelected);
	}
}
