// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_StoreTriggerBox.h"
#include "HG_StoreWidget.h"
#include "Components/BoxComponent.h"

// Sets default values
AHG_StoreTriggerBox::AHG_StoreTriggerBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionComp"));
	SetRootComponent(BoxCollisionComp);
	BoxCollisionComp->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
}

// Called when the game starts or when spawned
void AHG_StoreTriggerBox::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHG_StoreTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHG_StoreTriggerBox::CreateStoreWidget()
{
	if (StoreWidgetFactory)
	{
		StoreWidget = CreateWidget<UHG_StoreWidget>(GetWorld(), StoreWidgetFactory);
		if (StoreWidget)
		{
			StoreWidget->AddToViewport();
			bToggle = true;
		}
	}
}

void AHG_StoreTriggerBox::ByInteraction()
{
	if (nullptr == StoreWidget)
	{
		CreateStoreWidget();
	}
	else
	{
		if (!bToggle)
		{
			StoreWidget->SetVisibility(ESlateVisibility::Visible);
			bToggle = true;
		}
		else
		{
			StoreWidget->SetVisibility(ESlateVisibility::Hidden);

			bToggle = false;
		}
	}
}


