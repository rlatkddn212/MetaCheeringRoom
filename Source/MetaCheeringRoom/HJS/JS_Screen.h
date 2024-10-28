// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_Screen.generated.h"

USTRUCT()
struct FVedioInfo
{
	GENERATED_BODY()

public:
	
	FVedioInfo();

	FVedioInfo(bool blive, FString time, FString title, FString owner, FString streamURL, UTexture2D* thumbnail, FString category);

	UPROPERTY()
	bool bLive;
	UPROPERTY()
	FString Time;
	UPROPERTY()
	FString Title;
	UPROPERTY()
	FString Owner;
	UPROPERTY()
	FString StreamURL;
	UPROPERTY()
	UTexture2D* Thumbnail;

	UPROPERTY()
	FString Category;
};

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
	UPROPERTY(EditAnywhere)
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
	UPROPERTY(EditAnywhere)
	class UMediaSoundComponent* MediaSound2;

	TArray<FVedioInfo> VedioInfoList;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UVideoWidget> VideoWidgetFactory;
	
	UPROPERTY()
	class UVideoWidget* VideoWidget;

	UFUNCTION()
	void ClearVedioInfo();
	void AddVedioInfo(FVedioInfo Info);
	
	UFUNCTION()
	void OnMediaEndReached();
	UFUNCTION()
	void PlayMedia(const FString& VideoURL);

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastPlayMedia(const FString& VideoURL);
	
	UFUNCTION()
	void PlayVOD(const FString& VideoURL);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayVOD(const FString& VideoURL);
	
	UFUNCTION()
	void RequestMediaURL(FString URL);
	bool bUsingFirstPlayer = true; // 현재 어느 플레이어가 사용 중인지 체크
	void PrepareNextMediaSource();
};