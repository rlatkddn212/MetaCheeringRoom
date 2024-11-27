// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_CoinActor.h"
#include "GameFramework/Character.h"
#include "../SHK/HG_Player.h"
#include "MetaCheeringRoom.h"
#include "Components/SkeletalMeshComponent.h"
// Sets default values
AJS_CoinActor::AJS_CoinActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
}

// Called when the game starts or when spawned
void AJS_CoinActor::BeginPlay()
{
	Super::BeginPlay();
	Player = GetWorld()->GetFirstPlayerController()->GetCharacter();
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &AJS_CoinActor::ComponentBeginOverlap);

}

// Called every frame
void AJS_CoinActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Player)
	{
		if (bWayPoint == false)
		{
			FVector Dir = Player->GetMesh()->GetSocketLocation(TEXT("Head")) - GetActorLocation();
			Dir.Z += 70;
			Dir.Normalize();
			SetActorLocation(GetActorLocation() + Dir * 500.f * DeltaTime);
			Dir = Player->GetMesh()->GetSocketLocation(TEXT("Head"));
			Dir.Z += 70;
			if ((Dir - GetActorLocation()).Length() < 10.f)
			{
				bWayPoint = true;
			}
		}
		else
		{
			FVector Dir = Player->GetMesh()->GetSocketLocation(TEXT("Head")) - GetActorLocation();
			Dir.Normalize();
			SetActorLocation(GetActorLocation() + Dir * 500.f * DeltaTime);
		}

	}

}

void AJS_CoinActor::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AHG_Player>())
	{
		// 나이아가라 이펙트를 터뜨리기(플레이어 머리 위 위치에)

		// 디스트로이 하기
		Destroy();
	}
}

