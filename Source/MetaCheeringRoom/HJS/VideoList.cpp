// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/VideoList.h"

void UVideoList::NativeConstruct()
{
	Super::NativeConstruct();
	NodeArr.Add(BD_Node1);
	NodeArr.Add(BD_Node2);
	NodeArr.Add(BD_Node3);
	NodeArr.Add(BD_Node4);
}
