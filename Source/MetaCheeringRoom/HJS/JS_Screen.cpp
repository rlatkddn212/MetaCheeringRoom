// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_Screen.h"
#include "JS_NetComponent.h"
#include "MediaPlayer.h"
#include "MediaSoundComponent.h"
#include "MediaTexture.h"
#include "MetaCheeringRoom.h"
#include "StreamMediaSource.h"
#include "VideoWidget.h"

// Sets default values
AJS_Screen::AJS_Screen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ScreenComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScreenComp"));
	SetRootComponent(ScreenComp);
	NetComp = CreateDefaultSubobject<UJS_NetComponent>(TEXT("NetComp"));
	MediaSound = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("MediaSound"));
	MediaSound->SetupAttachment(RootComponent);
	MediaSound2 = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("MediaSound2"));
	MediaSound2->SetupAttachment(RootComponent);

	//bReplicates = true;
}

// Called when the game starts or when spawned
void AJS_Screen::BeginPlay()
{
	Super::BeginPlay();

	NetComp->Me = this;
	MediaPlayer->OnEndReached.AddDynamic(this, &AJS_Screen::OnMediaEndReached);
	MediaPlayer2->OnEndReached.AddDynamic(this, &AJS_Screen::OnMediaEndReached);
	MediaPlayer->PlayOnOpen = true;
	MediaPlayer2->PlayOnOpen = false;
	MediaPlayer->SetLooping(false);
	MediaPlayer2->SetLooping(false);

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if(PC)
	{
		SetOwner(PC);
		if (PC->HasAuthority())
		{
			if (MediaSound)
			{
				MediaSound->SetMediaPlayer(MediaPlayer);
			}
			if (MediaSound2)
			{
				MediaSound2->SetMediaPlayer(MediaPlayer2);
			}
			if (VideoWidgetFactory)
			{
				VideoWidget = CreateWidget<UVideoWidget>(GetWorld(),VideoWidgetFactory);
				if (VideoWidget)
				{
					VideoWidget->AddToViewport();
					//VideoWidget->SetVisibility(ESlateVisibility::Hidden);
				}
			}
			// 치지직 정보 가져오기
			NetComp->GetInfoFromAIServer();
			// 유튜브 정보 가져오기
			// VOD 정보 가져오기

			// 다 가져와서 어딘가에 리스트로 저장해야 함. 구조체를 하나 만들기
		}
	}
	
}

// Called every frame
void AJS_Screen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AJS_Screen::ClearVedioInfo()
{
	VedioInfoList.Empty();
}

void AJS_Screen::AddVedioInfo(FVedioInfo Info)
{
	VedioInfoList.Add(Info);
}

void AJS_Screen::OnMediaEndReached()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	//if (PC->HasAuthority())
	//{
		PrepareNextMediaSource();
		MediaPlayer->PlayOnOpen = false;
		MediaSource->StreamUrl = NetComp->VideoURL + TEXT(".mp4");
		if (bUsingFirstPlayer)
		{
			MediaTexture->SetMediaPlayer(MediaPlayer2);
			MediaPlayer2->Play();
			bUsingFirstPlayer = false;
		}
		else
		{
			MediaTexture->SetMediaPlayer(MediaPlayer);
			MediaPlayer->Play();
			bUsingFirstPlayer = true;
		}
	//}
}

void AJS_Screen::RequestMediaURL(FString URL)
{
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

void AJS_Screen::PlayMedia(const FString& VideoURL)
{
	MultiCastPlayMedia(VideoURL);
}

void AJS_Screen::MultiCastPlayMedia_Implementation(const FString& VideoURL)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	// Media를 재생
	// MediaSource의 URL 설정
	MediaTexture->SetMediaPlayer(MediaPlayer);
	if (MediaSource)
	{
		PRINTLOG(TEXT("%s"), *NetComp->VideoURL);
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

FVedioInfo::FVedioInfo()
{
	
}

FVedioInfo::FVedioInfo(bool blive, FString time, FString title, FString owner, FString streamURL, UTexture2D* thumbnail)
{
	bLive = blive;
	Time = time;
	Title = title;
	Owner = owner;
	Thumbnail = thumbnail;
	StreamURL =streamURL;
}
