// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Delegates/DelegateCombinations.h"
#include "JS_NetComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSummaryResultSignature, const FString&, Result);

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
	UPROPERTY(EditAnywhere)
	FString ServerURL = "http://121.165.108.133:13579";

	// 생성 Key
	FString StreamURL = TEXT("/convert_stream");

	// 하트비트 Key
	FString HeartBeatURL = TEXT("/heartbeat");

	// 생성된 URL
	FString StreamResURL;

	// 스트림 Unique ID
	FString StreamID;

	FString ClientID = "";

	void SendHeartBeat();

	// Video Key
	FString VideoURL = TEXT("");

	int32 SegmentNumber;

	FTimerHandle GetVedioTimerHandle;

	FString MakeJson(const TMap<FString, FString> source);

	FString JsonParseURLData(const FString& json);

	void GetVideoTimer();

	void GetVideoURL();

	UFUNCTION()
	void Play();

	UFUNCTION()
	void SetVideoURL();

	UFUNCTION()
	void SetVodURL();

	UFUNCTION()
	void GetInfoFromAIServer();

	void ParseChzzkVideoData(const FString& json);
	void ParseYoutubeVideoData(const FString& json);
	void ParseVODVideoData(const FString& json);

	UPROPERTY(EditAnywhere)
	UTexture2D* AdultOnlyTexture;

	UFUNCTION(NetMulticast,Reliable)
	void MulticastVideoURLWrite(const FString& URL, const FString& streamID,int32 segmentNumber);

	void RepVideoURLWrite(const FString& URL, const FString& streamID, int32 segmentNumber);

	FTimerHandle VideoInfoSettingHaldle;

	void VideoInfoSetting();

	void GetThumbnail(FString URL, FString title, FString channel, FString streamURL, FString time, FString category);

	void SendSummaryRequestVOD(FString StartTime, FString EndTime);

	FString CurrentVODFileName = TEXT("");

	FSummaryResultSignature OnSummarySignatureCompleteDelegate;

	void ShowResult();

};