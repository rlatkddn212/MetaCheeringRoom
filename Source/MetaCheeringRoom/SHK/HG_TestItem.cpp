// Fill out your copyright notice in the Description page of Project Settings.


#include "SHK/HG_TestItem.h"

void AHG_TestItem::BeginPlay()
{
	Super::BeginPlay();
	this->ItemData.ItemName = "TestItem";
	InitItemData();
}

void AHG_TestItem::Use()
{
	UE_LOG(LogTemp,Warning,TEXT("TestItem"));
}