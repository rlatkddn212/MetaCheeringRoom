// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_CreatorWidget.h"
#include "Ksw/CreatorPlayerController.h"
#include "GameFramework/Controller.h"

void USW_CreatorWidget::NativeConstruct()
{
	NativeConstruct();

	// PlayerController 가져오기
	ControllerReference = Cast<ACreatorPlayerController>(GetWorld()->GetFirstPlayerController());
}
