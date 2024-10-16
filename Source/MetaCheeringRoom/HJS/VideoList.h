// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JS_Screen.h"
#include "VideoList.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UVideoList : public UUserWidget
{
	GENERATED_BODY()
public:
	
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWIdget))
	class UHorizontalBox* HB_VedioList;

	UPROPERTY(meta=(BindWIdget))
	class UBorder* BD_Node1;
	UPROPERTY(meta=(BindWIdget))
	class UBorder* BD_Node2;
	UPROPERTY(meta=(BindWIdget))
	class UBorder* BD_Node3;
	UPROPERTY(meta=(BindWIdget))
	class UBorder* BD_Node4;

	UPROPERTY()
	TArray<class UBorder*> NodeArr;

};
