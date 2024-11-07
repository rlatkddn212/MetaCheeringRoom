// Fill out your copyright notice in the Description page of Project Settings.


#include "RLFProgress.h"

void URLFProgress::CalProgress(float f)
{
	if (progressCal.IsBound())progressCal.Broadcast(filePath,f);
}

void URLFProgress::CalFinish(AActor* actor)
{
	if (finishCal.IsBound())finishCal.Broadcast(filePath,actor);
}
