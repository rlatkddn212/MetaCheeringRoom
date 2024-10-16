// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_CreatorWidget.h"
#include "Ksw/CreatorPlayerController.h"
#include "GameFramework/Controller.h"

void USW_CreatorWidget::NativeConstruct()
{
	NativeConstruct();

	// PlayerController ��������
	ControllerReference = Cast<ACreatorPlayerController>(GetWorld()->GetFirstPlayerController());
}
