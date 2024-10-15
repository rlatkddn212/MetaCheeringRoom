// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_Screen.h"
#include "JS_NetComponent.h"
#include "MediaPlayer.h"
#include "MediaSoundComponent.h"
#include "MediaTexture.h"
#include "StreamMediaSource.h"

// Sets default values
AJS_Screen::AJS_Screen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NetComp = CreateDefaultSubobject<UJS_NetComponent>(TEXT("NetComp"));

	ScreenComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScreenComp"));

	MediaSound = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("MediaSound"));
	MediaSound->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AJS_Screen::BeginPlay()
{
	Super::BeginPlay();
	NetComp->Me = this;
	MediaPlayer->OnEndReached.AddDynamic(this, &AJS_Screen::OnMediaEndReached);
	MediaPlayer2->OnEndReached.AddDynamic(this, &AJS_Screen::OnMediaEndReached);

	if (MediaSound)
	{
		MediaSound->SetMediaPlayer(MediaPlayer);
	}
	MediaPlayer->PlayOnOpen = true;
	MediaPlayer2->PlayOnOpen = false;
	MediaPlayer->SetLooping(false);
	MediaPlayer2->SetLooping(false);
}

// Called every frame
void AJS_Screen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJS_Screen::OnMediaEndReached()
{
	PrepareNextMediaSource();
	UE_LOG(LogTemp,Warning,TEXT("===================%d"), bUsingFirstPlayer);
	MediaPlayer->PlayOnOpen = false;
	MediaSource->StreamUrl = NetComp->VideoURL + TEXT(".mp4");
	if (bUsingFirstPlayer)
	{
		MediaSound->SetMediaPlayer(MediaPlayer2);
		MediaTexture->SetMediaPlayer(MediaPlayer2);
		MediaPlayer2->Play();
		bUsingFirstPlayer = false;
	}
	else
	{
		MediaSound->SetMediaPlayer(MediaPlayer);
		MediaTexture->SetMediaPlayer(MediaPlayer);
		MediaPlayer->Play();
		bUsingFirstPlayer = true;
	}
}

void AJS_Screen::RequestMediaURL(FString URL)
{
	URL = TEXT("https://play.sooplive.co.kr/tjrdbs999/277798062");
	NetComp->URLSendToAIServer(URL);
}

void AJS_Screen::PrepareNextMediaSource()
{
	NetComp->SetVideoURL();
	if (bUsingFirstPlayer)
	{
		if (MediaSource)
		{
			MediaSource->StreamUrl = NetComp->VideoURL + TEXT(".mp4");
			if (MediaPlayer)
			{
				MediaPlayer->OpenSource(MediaSource);
			}
		}
	}
	else
	{
		if (MediaSource2)
		{
			MediaSource2->StreamUrl = NetComp->VideoURL + TEXT(".mp4");
			if (MediaPlayer2)
			{
				MediaPlayer2->OpenSource(MediaSource2);
			}
		}
	}
}

void AJS_Screen::PlayMedia()
{
	// Media를 재생
	// MediaSource의 URL 설정
	MediaTexture->SetMediaPlayer(MediaPlayer);
	if (MediaSource)
	{
		MediaSource->StreamUrl = NetComp->VideoURL + TEXT(".mp4");
		// 미디어 재생 시작
		if (MediaPlayer)
		{
			MediaPlayer->OpenSource(MediaSource);
		}
	}
	if (MediaSource2)
	{
		NetComp->SetVideoURL();
		MediaSource2->StreamUrl = NetComp->VideoURL + TEXT(".mp4");
		if (MediaPlayer2)
		{
			MediaPlayer2->OpenSource(MediaSource2);
		}
	}
}

// m3u8 파일을 읽어오기 ( URL : http://127.0.0.1:5000/stream/26644104-de2a-4ace-acc8-39f040012117.m3u8 ) 
// 26644104-de2a-4ace-acc8-39f040012117.m3u8 ( 고유 스트림 ID )

// 이 파일을 메모장으로 여는 것처럼 문자열을 싹다 가져오기
/*
#EXTM3U
#EXT-X-VERSION:3
#EXT-X-TARGETDURATION:10
#EXT-X-MEDIA-SEQUENCE:1037
#EXTINF:10.000000,
26644104-de2a-4ace-acc8-39f0400121171037.ts
#EXTINF:10.000000,
26644104-de2a-4ace-acc8-39f0400121171038.ts
#EXTINF:10.000000,
26644104-de2a-4ace-acc8-39f0400121171039.ts
#EXTINF:10.000000,
26644104-de2a-4ace-acc8-39f0400121171040.ts
#EXTINF:10.000000,
26644104-de2a-4ace-acc8-39f0400121171041.ts
*/
// 파일 예시는 다음과 같음.
// 여기에 있는 가장 첫 번째 ID 26644104-de2a-4ace-acc8-39f040012117의 위치 찾는다.
// 그 시작 위치부터 .ts를 만나기 전까지의 문자열에서,26644104-de2a-4ace-acc8-39f040012117 이후에 있는 문자들 저장( Int32로 바꿔서 )
// EX ) 26644104-de2a-4ace-acc8-39f0400121171037 이라면 1037을 저장

// 이 URL http://127.0.0.1:5000/videos/26644104-de2a-4ace-acc8-39f0400121171037에 이전에 저장해 두었던 int32로 바꾸었던 숫자를 더하여 문자열 만들기
// 뒤 문자까지 합해진 URL의 링크를 MediaSource ( UStreamMediaSource ) 에 URL로 설정
