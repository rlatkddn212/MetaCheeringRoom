// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_NetComponent.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include "HttpFwd.h"
#include "JS_Screen.h"
#include "IImageWrapperModule.h"
#include "VideoWidget.h"
#include "Net/UnrealNetwork.h"
#include "MetaCheeringRoom.h"
#include "MediaPlayer.h"

// Sets default values for this component's properties
UJS_NetComponent::UJS_NetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void UJS_NetComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	FTimerHandle HeartBeatHandle;
	GetWorld()->GetTimerManager().SetTimer(HeartBeatHandle,this, &UJS_NetComponent::SendHeartBeat,5,true);
}


// Called every frame
void UJS_NetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UJS_NetComponent::URLSendToAIServer(const FString& URL)
{
	// 파일 읽기
	TMap<FString, FString> Senddata;
	Senddata.Add(TEXT("url"), URL);

	// HTTP 요청 생성
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(ServerURL + StreamURL);
	HttpRequest->SetVerb("POST");
	HttpRequest->SetHeader(TEXT("content-type"), TEXT("application/json"));
	HttpRequest->SetContentAsString(MakeJson(Senddata));

	// HTTP 응답 처리
	HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (bWasSuccessful && Response->GetResponseCode() == 200)
			{
				FString Res = Response->GetContentAsString();
				StreamResURL = JsonParseURLData(Res);
				StreamResURL.Split(TEXT("/stream/"), nullptr, &StreamID);
				StreamID.Split(TEXT(".m3u8"), &StreamID, nullptr);
				this->GetVideoTimer();
				UE_LOG(LogTemp, Error, TEXT("성공"));
				this->SendHeartBeat();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("실패"));
			}
		});

	// 요청 전송
	HttpRequest->ProcessRequest();
}

void UJS_NetComponent::SendHeartBeat()
{
	if (ClientID == "")
	{
		return;
	}

	// 파일 읽기
	TMap<FString, FString> Senddata;
	Senddata.Add(TEXT("client_id"), ClientID);

	// HTTP 요청 생성
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(ServerURL + HeartBeatURL);
	HttpRequest->SetVerb("POST");
	HttpRequest->SetHeader(TEXT("content-type"), TEXT("application/json"));
	HttpRequest->SetContentAsString(MakeJson(Senddata));

	// HTTP 응답 처리
	HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (bWasSuccessful && Response->GetResponseCode() == 200)
			{
			}
		});

	// 요청 전송
	HttpRequest->ProcessRequest();
}

FString UJS_NetComponent::MakeJson(const TMap<FString, FString> source)
{
	// source를 JsonObject 형식으로 만든다.
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());

	for (TPair<FString, FString> pair : source)
	{
		jsonObject->SetStringField(pair.Key, pair.Value);
	}

	// writer를 만들어서 JsonObject를 인코딩
	FString json;
	TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<TCHAR>::Create(&json);
	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), writer);

	return json;
}

FString UJS_NetComponent::JsonParseURLData(const FString& json)
{
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
	TSharedPtr<FJsonObject> response = MakeShareable(new FJsonObject());

	FString result;
	if (FJsonSerializer::Deserialize(reader, response))
	{
		result = response->GetStringField(TEXT("url"));
		ClientID = response->GetStringField(TEXT("client_id"));
	}
	return result;
}

void UJS_NetComponent::GetVideoTimer()
{
	GetWorld()->GetTimerManager().SetTimer(GetVedioTimerHandle, this, &UJS_NetComponent::GetVideoURL, 3.f, true);
}

void UJS_NetComponent::GetVideoURL()
{
	FString RequestURL = ServerURL + StreamResURL;
	// M3U8 파일을 다운로드하는 HTTP 요청 생성

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		if (bWasSuccessful && Response->GetResponseCode() == 200)
		{
			FString Res = Response->GetContentAsString();

			SegmentNumber = -1;

			FString SegmentPrefix = StreamID;

			int32 Index = Res.Find(SegmentPrefix);
			if (Index != INDEX_NONE)
			{
				// Stream ID 이후의 문자열에서 세그먼트 번호 추출
				FString Substring = Res.Mid(Index + SegmentPrefix.Len());
				FString SegmentStr;
				Substring.Split(TEXT(".ts"), &SegmentStr, nullptr);
				SegmentNumber = FCString::Atoi(*SegmentStr);
			}

			if (SegmentNumber == -1)
			{
				UE_LOG(LogTemp, Error, TEXT("M3U8 파일에서 세그먼트 번호 추출 실패"));
				return;
			}
			VideoURL = FString::Printf(TEXT("%s/videos/%s%d"),*ServerURL,*StreamID,SegmentNumber);
			this->MulticastVideoURLWrite(VideoURL, StreamID, SegmentNumber);
			UE_LOG(LogTemp, Log, TEXT("재생할 비디오 URL: %s"), *VideoURL);
			GetWorld()->GetTimerManager().ClearTimer(GetVedioTimerHandle);
			FTimerHandle StartMediaHandle;
			Me->MediaPlayer->Pause();
			Me->MediaPlayer2->Pause();
			GetWorld()->GetTimerManager().SetTimer(StartMediaHandle,this,&UJS_NetComponent::Play,10.f,false);

		}
		else
		{
			
		}
	});
	HttpRequest->SetURL(RequestURL);
	HttpRequest->SetVerb("GET");
	HttpRequest->ProcessRequest();
}

void UJS_NetComponent::Play()
{
	Me->PlayMedia(VideoURL);
}

void UJS_NetComponent::SetVideoURL()
{
	VideoURL = FString::Printf(TEXT("%s/videos/%s%d"), *ServerURL, *StreamID, ++SegmentNumber);
	UE_LOG(LogTemp, Log, TEXT("재생할 비디오 URL: %s"), *VideoURL);
}

void UJS_NetComponent::SetVodURL()
{
	
}

void UJS_NetComponent::GetInfoFromAIServer()
{	// 파일 읽기
	// HTTP 요청 생성
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(ServerURL + TEXT("/chzzk_live_streams"));
	HttpRequest->SetVerb("GET");

	// HTTP 응답 처리
	HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (bWasSuccessful && Response->GetResponseCode() == 200)
			{
				FString Res = Response->GetContentAsString();
				ParseChzzkVideoData(Res);
				UE_LOG(LogTemp, Warning, TEXT("성공"));
			}
			else
			{
				UE_LOG(LogTemp,Warning,TEXT("실패"));
			}
		});

	// 요청 전송
	HttpRequest->ProcessRequest();

	HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(ServerURL + TEXT("/youtube_live_streams"));
	HttpRequest->SetVerb("GET");

	// HTTP 응답 처리
	HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (bWasSuccessful && Response->GetResponseCode() == 200)
			{
				FString Res = Response->GetContentAsString();
				ParseYoutubeVideoData(Res);
				UE_LOG(LogTemp, Warning, TEXT("성공"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("실패"));
			}
		});

	// 요청 전송
	HttpRequest->ProcessRequest();

	// 요청 전송
	HttpRequest->ProcessRequest();

	HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(ServerURL + TEXT("/vod_list"));
	HttpRequest->SetVerb("GET");

	// HTTP 응답 처리
	HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (bWasSuccessful && Response->GetResponseCode() == 200)
			{
				FString Res = Response->GetContentAsString();
				ParseVODVideoData(Res);
				UE_LOG(LogTemp, Warning, TEXT("성공"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("실패"));
			}
		});

	// 요청 전송
	HttpRequest->ProcessRequest();
}

void UJS_NetComponent::ParseChzzkVideoData(const FString& json)
{
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
	TSharedPtr<FJsonObject> response = MakeShareable(new FJsonObject());
	
	if (FJsonSerializer::Deserialize(reader, response))
	{
		TArray<TSharedPtr<FJsonValue>> LiveStreams = response->GetArrayField(TEXT("live_streams"));
		int count = 0;
		for (TSharedPtr<FJsonValue> StreamValue : LiveStreams)
		{
			if (count >= 10)
			{
				break;
			}
			count++;
			TSharedPtr<FJsonObject> StreamObject = StreamValue->AsObject();
			if (StreamObject.IsValid())
			{
				FString title = StreamObject->GetStringField(TEXT("Title"));
				FString channel = StreamObject->GetStringField(TEXT("channel"));
				FString thumbnail = StreamObject->GetStringField(TEXT("thumbnail"));
				FString streamURL = StreamObject->GetStringField(TEXT("stream_url"));
				FString time = StreamObject->GetStringField(TEXT("viewers"));

				if (thumbnail == "" && AdultOnlyTexture)
				{
					Me->VedioInfoList.Add(FVedioInfo(true, time, title, channel, streamURL, AdultOnlyTexture, TEXT("Chzzk")));
					continue;
				}

				GetThumbnail(thumbnail,title,channel,streamURL,time,TEXT("Chzzk"));
			}
		}
	}
}

void UJS_NetComponent::ParseYoutubeVideoData(const FString& json)
{
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
	TSharedPtr<FJsonObject> response = MakeShareable(new FJsonObject());

	if (FJsonSerializer::Deserialize(reader, response))
	{
		TArray<TSharedPtr<FJsonValue>> LiveStreams = response->GetArrayField(TEXT("live_streams"));

		for (TSharedPtr<FJsonValue> StreamValue : LiveStreams)
		{
			TSharedPtr<FJsonObject> StreamObject = StreamValue->AsObject();
			if (StreamObject.IsValid())
			{
				FString title = StreamObject->GetStringField(TEXT("title"));
				FString channel = StreamObject->GetStringField(TEXT("channelTitle"));
				FString thumbnail = StreamObject->GetStringField(TEXT("liveThumbnail"));
				FString streamURL = StreamObject->GetStringField(TEXT("watchUrl"));
				FString time = TEXT("Live");
				
				GetThumbnail(thumbnail, title, channel, streamURL, time, TEXT("Youtube"));
			}
		}
	}
}

void UJS_NetComponent::ParseVODVideoData(const FString& json)
{
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
	TSharedPtr<FJsonObject> response = MakeShareable(new FJsonObject());
	if (FJsonSerializer::Deserialize(reader, response))
	{
		TArray<TSharedPtr<FJsonValue>> VODs = response->GetArrayField(TEXT("vod_files"));

		for (TSharedPtr<FJsonValue> VideoValue : VODs)
		{
			TSharedPtr<FJsonObject> VideoObject = VideoValue->AsObject();
			if (VideoObject.IsValid())
			{
				FString title = VideoObject->GetStringField(TEXT("filename"));
				FString channel = TEXT("VOD");
				FString thumbnail = ServerURL + VideoObject->GetStringField(TEXT("thumbnail"));
				FString streamURL = ServerURL + TEXT("/vod/") + VideoObject->GetStringField(TEXT("filename"));
				FString time = VideoObject->GetStringField(TEXT("duration"));

				GetThumbnail(thumbnail, title, channel, streamURL, time, TEXT("VOD"));
			}
		}
	}
}

void UJS_NetComponent::MulticastVideoURLWrite_Implementation(const FString& URL, const FString& streamID, int32 segmentNumber)
{
	VideoURL = URL;
	StreamID = streamID;
	SegmentNumber = segmentNumber;
	PRINTLOG(TEXT("%s"), *VideoURL);
}

void UJS_NetComponent::RepVideoURLWrite(const FString& URL, const FString& streamID, int32 segmentNumber)
{
	VideoURL = URL;
	StreamID = streamID;
	SegmentNumber = segmentNumber;
}

void UJS_NetComponent::VideoInfoSetting()
{
	if (Me)
	{
		if (Me->VideoWidget)
		{
			Me->VideoWidget->SettingLiveInfo(Me->VedioInfoList);
		}
	}
}

void UJS_NetComponent::GetThumbnail(FString URL, FString title, FString channel, FString streamURL, FString time, FString category)
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(URL);
	HttpRequest->SetVerb("GET");
	HttpRequest->OnProcessRequestComplete().BindLambda([this, title, channel, streamURL, time, category](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (bWasSuccessful && Response.IsValid())
			{
				const TArray<uint8>& ImageData = Response->GetContent();
				// 이미지 포맷이 JPG/PNG인지 파악하고 적절히 디코딩
				IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
				TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
				if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(ImageData.GetData(), ImageData.Num()))
				{
					TArray<uint8> RawImageData;
					if (ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, RawImageData))
					{
						UTexture2D* NewTexture = UTexture2D::CreateTransient(
							ImageWrapper->GetWidth(),
							ImageWrapper->GetHeight(),
							PF_R8G8B8A8
						);
						FTexture2DMipMap& Mip = NewTexture->GetPlatformData()->Mips[0];
						Mip.SizeX = ImageWrapper->GetWidth();
						Mip.SizeY = ImageWrapper->GetHeight();
						Mip.BulkData.Lock(LOCK_READ_WRITE);
						// 텍스처에 이미지 데이터 복사
						void* TextureData = Mip.BulkData.Realloc(RawImageData.Num());
						FMemory::Memcpy(TextureData, RawImageData.GetData(), RawImageData.Num());
						Mip.BulkData.Unlock();
						NewTexture->UpdateResource();
						if (category == TEXT("VOD"))
						{
							Me->VedioInfoList.Add(FVedioInfo(false, time, title, channel, streamURL, NewTexture, category));
						}
						else
						{
							Me->VedioInfoList.Add(FVedioInfo(true, time, title, channel, streamURL, NewTexture, category));
						}
						VideoInfoSetting();
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("실패"));
			}
		});
	HttpRequest->ProcessRequest();
}

void UJS_NetComponent::SendSummaryRequestVOD(FString StartTime, FString EndTime)
{

	TMap<FString, FString> Senddata;

	Senddata.Add(TEXT("VedioFileName"), CurrentVODFileName);
	Senddata.Add(TEXT("StartTime"), StartTime);
	Senddata.Add(TEXT("EndTime"), EndTime);

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(ServerURL + TEXT("/save_times"));
	HttpRequest->SetVerb("POST");
	HttpRequest->SetHeader(TEXT("content-type"), TEXT("application/json"));
	HttpRequest->SetContentAsString(MakeJson(Senddata));

	// HTTP 응답 처리
	HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (bWasSuccessful && Response->GetResponseCode() == 200)
			{
				FString Res = Response->GetContentAsString();
				TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(Res);
				TSharedPtr<FJsonObject> response = MakeShareable(new FJsonObject());

				FString result;
				if (FJsonSerializer::Deserialize(reader, response))
				{
					result = response->GetStringField(TEXT("Summary"));
				}
				// 브로드캐스트 하기
				OnSummarySignatureCompleteDelegate.Broadcast(result);
				UE_LOG(LogTemp, Error, TEXT("성공, 요약 결과 %s"), *result);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("실패"));
			}
		});

	// 요청 전송
	HttpRequest->ProcessRequest();
}
