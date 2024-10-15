// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_NetComponent.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include "HttpFwd.h"
#include "JS_Screen.h"

// Sets default values for this component's properties
UJS_NetComponent::UJS_NetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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
				GetVideoTimer();
				UE_LOG(LogTemp, Error, TEXT("성공"));
				SendHeartBeat();
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
	UE_LOG(LogTemp, Error, TEXT("전송 전"));
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
				UE_LOG(LogTemp, Error, TEXT("응답"));
			}
		});

	// 요청 전송
	HttpRequest->ProcessRequest();
	UE_LOG(LogTemp, Error, TEXT("전송 후"));
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
	FTimerHandle GetVedioTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(GetVedioTimerHandle, this, &UJS_NetComponent::GetVideoURL, 5.f, false);
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
			UE_LOG(LogTemp, Log, TEXT("재생할 비디오 URL: %s"), *VideoURL);

			FTimerHandle StartMediaHandle;
			GetWorld()->GetTimerManager().SetTimer(StartMediaHandle,this,&UJS_NetComponent::Play,20.f,false);

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
	Me->PlayMedia();
}

void UJS_NetComponent::SetVideoURL()
{
	VideoURL = FString::Printf(TEXT("%s/videos/%s%d"), *ServerURL, *StreamID, ++SegmentNumber);
	UE_LOG(LogTemp, Log, TEXT("재생할 비디오 URL: %s"), *VideoURL);
}

void UJS_NetComponent::SetVodURL()
{
	
}

