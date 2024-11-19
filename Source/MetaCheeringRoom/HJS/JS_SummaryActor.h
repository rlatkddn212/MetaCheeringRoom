// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_SummaryActor.generated.h"

UCLASS()
class METACHEERINGROOM_API AJS_SummaryActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_SummaryActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;

	UFUNCTION()
	void ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UJS_SumarryRequestWidget> SummaryReqWidgetFactory;
	
	UPROPERTY()
	class UJS_SumarryRequestWidget* SummaryReqWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UJS_SummaryResultWidget> SummaryResultWidgetFactory;

	UPROPERTY()
	class UJS_SummaryResultWidget* SummaryResultWidget;

	UFUNCTION(BlueprintCallable)
	void ShowSummaryWidget();
};
