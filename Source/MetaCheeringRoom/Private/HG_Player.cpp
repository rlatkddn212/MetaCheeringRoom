// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_Player.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/StaticMesh.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

// Sets default values
AHG_Player::AHG_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

// 	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
// 	SpringArmComp->SetupAttachment(RootComponent);
// 	SpringArmComp->TargetArmLength = 200.0f;
// 
// 	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
// 	CameraComp->SetupAttachment(SpringArmComp);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0.0f,0.0f,-80.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f,0.0f,-90.0f));
}

// Called when the game starts or when spawned
void AHG_Player::BeginPlay()
{
	Super::BeginPlay();
	
	auto* pc = Cast<APlayerController>(Controller);
	if (pc) 
	{
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSys)
		{
			subSys->AddMappingContext(IMC_Player,0);
		}
	}
}

// Called every frame
void AHG_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FTransform t = FTransform(GetControlRotation());
	Direction = t.TransformVector(Direction);

	AddMovementInput(Direction);
	Direction = FVector::ZeroVector;
}

// Called to bind functionality to input
void AHG_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	input->BindAction(IA_Move,ETriggerEvent::Triggered,this,&AHG_Player::OnMyMove);
	input->BindAction(IA_Jump,ETriggerEvent::Triggered,this,&AHG_Player::OnMyJump);
	input->BindAction(IA_Look,ETriggerEvent::Triggered,this,&AHG_Player::OnMyLook);

}

void AHG_Player::OnMyMove(const FInputActionValue& Value)
{
	FVector2D v = Value.Get<FVector2D>();
	Direction.X= v.X;
	Direction.Y = v.Y;
	Direction.Normalize();
}

void AHG_Player::OnMyJump(const FInputActionValue& Value)
{
	Jump();
}

void AHG_Player::OnMyLook(const FInputActionValue& Value)
{
	FVector2D v = Value.Get<FVector2D>();
	AddControllerPitchInput(-v.Y);
	AddControllerYawInput(v.X);
}

