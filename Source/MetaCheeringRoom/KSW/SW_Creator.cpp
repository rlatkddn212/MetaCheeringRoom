// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/SW_Creator.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"

// Sets default values
ASW_Creator::ASW_Creator()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASW_Creator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASW_Creator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// p = p0 + v * t

	FVector p = GetActorLocation();
	p += Direction * 500 * DeltaTime;
	SetActorLocation(p);

	Direction = FVector::ZeroVector;
}

// Called to bind functionality to input
void ASW_Creator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto* pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSys)
		{
			subSys->AddMappingContext(IMC_Player, 0);
		}
	}

	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// ¿òÁ÷ÀÓ
	input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ASW_Creator::OnMyMove);
	input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ASW_Creator::OnMyLook);
}

void ASW_Creator::OnMyMove(const FInputActionValue& Value)
{
	FVector2D v = Value.Get<FVector2D>();
	Direction.X = v.X;
	Direction.Y = v.Y;
	Direction.Normalize();
}

void ASW_Creator::OnMyLook(const FInputActionValue& Value)
{
	FVector2D v = Value.Get<FVector2D>();
	AddControllerPitchInput(-v.Y);
	AddControllerYawInput(v.X);
}

