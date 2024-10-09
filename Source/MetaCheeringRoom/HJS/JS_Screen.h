// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_Screen.generated.h"

UCLASS()
class METACHEERINGROOM_API AJS_Screen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_Screen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	class UJS_NetComponent* NetComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* ScreenComp;

	UPROPERTY(EditAnywhere)
	class UMediaPlayer* MediaPlayer;

	UPROPERTY(EditAnywhere)
	class UMediaPlayer* MediaPlayer2;

	UPROPERTY(EditAnywhere)
	class UMediaTexture* MediaTexture;

	UPROPERTY(EditAnywhere)
	class UStreamMediaSource* MediaSource;

	UPROPERTY(EditAnywhere)
	class UStreamMediaSource* MediaSource2;

	UPROPERTY(EditAnywhere)
	class UMediaSoundComponent* MediaSound;

	UFUNCTION()
	void OnMediaEndReached();

	UFUNCTION()
	void PlayMedia();

	UFUNCTION()
	void RequestMediaURL(FString URL);

	bool bUsingFirstPlayer = true; // 현재 어느 플레이어가 사용 중인지 체크

	void PrepareNextMediaSource();

};
