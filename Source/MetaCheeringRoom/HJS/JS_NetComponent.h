// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JS_NetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class METACHEERINGROOM_API UJS_NetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJS_NetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	class AJS_Screen* Me;
	UFUNCTION()
	void URLSendToAIServer(const FString& VideoURL);

	// 서버 URL
	const FString ServerURL = "http://127.0.0.1:5000/";

	// 생성 Key
	FString VideoURL = TEXT("convert_stream");

	// 스트림 Key
	FString StreamURL;

	FString MakeJson(const TMap<FString, FString> source);

	FString JsonParseURLData(const FString& json);
};
