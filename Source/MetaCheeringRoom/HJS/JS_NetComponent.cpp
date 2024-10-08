// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_NetComponent.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include "HttpFwd.h"

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
	HttpRequest->SetURL(ServerURL);
	HttpRequest->SetVerb("POST");
	HttpRequest->SetHeader(TEXT("content-type"), TEXT("application/json"));
	HttpRequest->SetContentAsString(MakeJson(Senddata));

	// HTTP 응답 처리
	HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (bWasSuccessful && Response->GetResponseCode() == 200)
			{
				FString Res = Response->GetContentAsString();
				StreamURL = JsonParseURLData(Res);
			}
			else
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
	}
	return result;
}

