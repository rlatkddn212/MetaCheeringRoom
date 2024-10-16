// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/SW_Creator.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "SW_CreatorPlayerController.h"

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
	FTransform t = FTransform(GetControlRotation());
	Direction = t.TransformVector(Direction);

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

	// 움직임
	input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ASW_Creator::OnMyMove);
	input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ASW_Creator::OnMyLook);

	input->BindAction(IA_Q, ETriggerEvent::Triggered, this, &ASW_Creator::OnMyQ);
	input->BindAction(IA_W, ETriggerEvent::Triggered, this, &ASW_Creator::OnMyW);
	input->BindAction(IA_E, ETriggerEvent::Triggered, this, &ASW_Creator::OnMyE);
	input->BindAction(IA_R, ETriggerEvent::Triggered, this, &ASW_Creator::OnMyR);
	input->BindAction(IA_RClick, ETriggerEvent::Triggered, this, &ASW_Creator::OnMyRClick);
	input->BindAction(IA_LClick, ETriggerEvent::Triggered, this, &ASW_Creator::OnMyLClick);
	input->BindAction(IA_LClick, ETriggerEvent::Started, this, &ASW_Creator::OnMyLClick);
	input->BindAction(IA_LClick, ETriggerEvent::Completed, this, &ASW_Creator::OnMyLClick);

	input->BindAction(IA_RClick, ETriggerEvent::Started, this, &ASW_Creator::OnMyRClickStarted);
	input->BindAction(IA_RClick, ETriggerEvent::Completed, this, &ASW_Creator::OnMyRClickCompleted);

	input->BindAction(IA_RClick, ETriggerEvent::Started, this, &ASW_Creator::OnMyLClickStarted);
	input->BindAction(IA_RClick, ETriggerEvent::Completed, this, &ASW_Creator::OnMyLClickCompleted);
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

void ASW_Creator::OnMyQ(const FInputActionValue& Value)
{
	Direction.Z = -1;
}

void ASW_Creator::OnMyW(const FInputActionValue& Value)
{

}

void ASW_Creator::OnMyE(const FInputActionValue& Value)
{
	// 위로 이동
	Direction.Z = 1;
}

void ASW_Creator::OnMyR(const FInputActionValue& Value)
{

}

void ASW_Creator::OnMyRClick(const FInputActionValue& Value)
{

}

void ASW_Creator::OnMyLClick(const FInputActionValue& Value)
{
	
}

void ASW_Creator::OnMyRClickStarted(const FInputActionValue& Value)
{
	
}

void ASW_Creator::OnMyRClickCompleted(const FInputActionValue& Value)
{

}

void ASW_Creator::OnMyLClickStarted(const FInputActionValue& Value)
{
	auto* pc = Cast<ASW_CreatorPlayerController>(Controller);
	if (pc)
	{
		pc->OnLeftClick();
	}
}

void ASW_Creator::OnMyLClickCompleted(const FInputActionValue& Value)
{

}
