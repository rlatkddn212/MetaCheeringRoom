// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/UI/SW_CreatorWidget.h"
#include "Ksw/SW_CreatorPlayerController.h"
#include "GameFramework/Controller.h"

void USW_CreatorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// PlayerController ��������
	//ControllerReference = Cast<ASW_CreatorPlayerController>(GetWorld()->GetFirstPlayerController());
}
