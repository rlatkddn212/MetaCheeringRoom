// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_EmojiItemBase.h"
#include "Net/UnrealNetwork.h"

AHG_EmojiItemBase::AHG_EmojiItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp->SetIsReplicated(true);

	bReplicates = true;
	SetReplicateMovement(true);
}

void AHG_EmojiItemBase::BeginPlay()
{
	Super::BeginPlay();

	this->ItemData.ItemName = Name;
	InitItemData();

	SetActorScale3D(FVector(0.1f, 0.1f, 0.000001f));

	SpawnTime = GetWorld()->GetTimeSeconds();

	SetLifeSpan(4.0f);
}

void AHG_EmojiItemBase::Use()
{
	ChangeMeshMaterial();
	ChangeMaterialTexture();
}

void AHG_EmojiItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 목표 스케일
	FVector TargetScale(1.0f, 1.0f, 0.000001f);

	// 현재 스케일 가져오기
	FVector CurrentScale = GetActorScale3D();

	// 보간 속도 조절 (0에서 1 사이의 값, 클수록 빠름)
	float LerpSpeed = 0.1f;

	// 보간을 통해 새로운 스케일 계산
	FVector NewScale = FMath::Lerp(CurrentScale, TargetScale, LerpSpeed);

	// 새로운 스케일 적용
	SetActorScale3D(NewScale);


	float ElapsedTime = GetWorld()->GetTimeSeconds() - SpawnTime;
	if (ElapsedTime > 3.2f)
	{
		NewScale = FMath::VInterpTo(GetActorScale3D(), FVector(-1.0f, -1.0f, 0.000001f), DeltaTime,5.0f);

		SetActorScale3D(NewScale);
	}

	FollowPlayer();
}

void AHG_EmojiItemBase::ServerRPC_ChangeMeshMaterial_Implementation()
{
	MulticastRPC_ChangeMeshMaterial();
}

void AHG_EmojiItemBase::MulticastRPC_ChangeMeshMaterial_Implementation()
{
	UMaterialInterface* NewMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/SHK/Material/M_Emoji.M_Emoji"));
	if (NewMaterial && MeshComp)
	{
		MeshComp->SetMaterial(0, NewMaterial);
	}
}

void AHG_EmojiItemBase::ChangeMeshMaterial()
{
	ServerRPC_ChangeMeshMaterial();
}

void AHG_EmojiItemBase::ServerRPC_ChangeMaterialTexture_Implementation()
{
	MulticastRPC_ChangeMaterialTexture();
}

void AHG_EmojiItemBase::MulticastRPC_ChangeMaterialTexture_Implementation()
{
	if (MeshComp)
	{
		UMaterialInstanceDynamic* DynamicMaterial = MeshComp->CreateAndSetMaterialInstanceDynamic(0);

		if (DynamicMaterial && ItemData.ItemIcon)
		{
			DynamicMaterial->SetTextureParameterValue(FName("BaseTexture"), ItemData.ItemIcon);
		}
	}
}

void AHG_EmojiItemBase::ChangeMaterialTexture()
{
	ServerRPC_ChangeMaterialTexture();
}

void AHG_EmojiItemBase::FollowPlayer()
{
	ServerRPC_FollowPlayer();
}

void AHG_EmojiItemBase::ServerRPC_FollowPlayer_Implementation()
{
	MulticastRPC_FollowPlayer();
}

void AHG_EmojiItemBase::MulticastRPC_FollowPlayer_Implementation()
{
	if (Owner)
	{
		SetActorLocation(Owner->GetActorLocation() + FVector(0.0f, 0.0f, 70.0f));
		SetActorRelativeRotation(Owner->GetActorRotation() + FRotator(90.0f, 0.0f, 0.0f));
		Loc = GetActorLocation();
		Rot = GetActorRotation();
	}
}

void AHG_EmojiItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHG_EmojiItemBase, Rot);
	DOREPLIFETIME(AHG_EmojiItemBase, Loc);
}