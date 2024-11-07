// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_Screen.h"
#include "../SHK/HG_PlayerGoodsComponent.h"
#include "../SHK/HG_Player.h"
#include "JS_NetComponent.h"
#include "MediaPlayer.h"
#include "MediaSoundComponent.h"
#include "MediaTexture.h"
#include "MetaCheeringRoom.h"
#include "StreamMediaSource.h"
#include "VideoWidget.h"
#include "JS_ExitWidget.h"
#include "Net/UnrealNetwork.h"
#include "JS_PlayerController.h"

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

	bReplicates = true;
}

// Called when the game starts or when spawned
void AJS_Screen::BeginPlay()
{
	Super::BeginPlay();

	NetComp->Me = this;
	MediaPlayer->OnEndReached.AddDynamic(this, &AJS_Screen::OnMediaEndReached);
	MediaPlayer2->OnEndReached.AddDynamic(this, &AJS_Screen::OnMediaEndReached);
	MediaPlayer->OnMediaOpenFailed.AddDynamic(this, &AJS_Screen::OnFailedLoadVideo1);
	MediaPlayer2->OnMediaOpenFailed.AddDynamic(this, &AJS_Screen::OnFailedLoadVideo2);
	MediaPlayer->OnMediaOpened.AddDynamic(this, &AJS_Screen::OnSucceedLoadVideo1);
	MediaPlayer2->OnMediaOpened.AddDynamic(this, &AJS_Screen::OnSucceedLoadVideo2);
	MediaPlayer->PlayOnOpen = true;
	MediaPlayer2->PlayOnOpen = false;
	MediaPlayer->SetLooping(false);
	MediaPlayer2->SetLooping(false);
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if(PC)
	{
		GetWorldTimerManager().SetTimer(PlayVedioHandle, this, &AJS_Screen::RepPlayVideo, 1.f, false);
		if (PC->HasAuthority())
		{
			SetOwner(PC);
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
					VideoWidget->AddToViewport(30);
					VideoWidget->SetVisibility(ESlateVisibility::Hidden);
				}
			}
			// 치지직 정보 가져오기
			NetComp->GetInfoFromAIServer();
			// 유튜브 정보 가져오기
			// VOD 정보 가져오기

			// 다 가져와서 어딘가에 리스트로 저장해야 함. 구조체를 하나 만들기
		}
	}
	Player = Cast<AHG_Player>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (Player)
	{
		if (Player->IsLocallyControlled())
		{
			Player->EnterTheStore();
			GetWorldTimerManager().SetTimer(AddPointTImerHandle, this, &AJS_Screen::AddPoint, 5.f, false);
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
	PrepareNextMediaSource();
	MediaPlayer->PlayOnOpen = false;
	MediaSource->StreamUrl = NetComp->VideoURL + TEXT(".mp4");
	if (bUsingFirstPlayer)
	{
		MediaTexture->SetMediaPlayer(MediaPlayer2);
		MediaPlayer2->Play();
		if (PC->HasAuthority())
		{
			RepVideoURL = MediaSource2->StreamUrl;
		}
		bUsingFirstPlayer = false;
	}
	else
	{
		MediaTexture->SetMediaPlayer(MediaPlayer);
		MediaPlayer->Play();
		if (PC->HasAuthority())
		{
			RepVideoURL = MediaSource->StreamUrl;
		}
		bUsingFirstPlayer = true;
	}
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
				PRINTLOG(TEXT("%d"), MediaPlayer->OpenSource(MediaSource));
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
				PRINTLOG(TEXT("%d"), MediaPlayer2->OpenSource(MediaSource2));
			}
		}
	}
}

void AJS_Screen::AddPoint()
{
	
	if (Player)
	{
		if (Player->IsLocallyControlled())
		{
			if (!Player->StoreWidget || !Player->bStoreWidget)
			{
				Player->EnterTheStore();
			}
			UHG_PlayerGoodsComponent* PG = Player->GoodsComp;
			int32 GoldWeight = PointArr[FMath::RandRange(0,PointArr.Num()-1)];
			PG->AddGold(GoldWeight);
			GetWorldTimerManager().SetTimer(AddPointTImerHandle,this,&AJS_Screen::AddPoint,5.f,false);
		}
	}
	else
	{
		Player = Cast<AHG_Player>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		GetWorldTimerManager().SetTimer(AddPointTImerHandle, this, &AJS_Screen::AddPoint, 1.f, false);
	}
}

void AJS_Screen::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AJS_Screen, RepVideoURL);
	DOREPLIFETIME(AJS_Screen, RepVideoURL2);
	DOREPLIFETIME(AJS_Screen, bUsingFirstPlayer);
}


void AJS_Screen::RepPlayVideo()
{
	PRINTLOG(TEXT("%s"), *RepVideoURL);
	if (RepVideoURL != TEXT(""))
	{
		NetComp->VideoURL = RepVideoURL;
		AJS_PlayerController* PC = Cast<AJS_PlayerController>(GetWorld()->GetFirstPlayerController());
		if (PC)
		{
			PC->ServerHandleVideoPlay();
		}
	}
}

void AJS_Screen::OnFailedLoadVideo1(FString FailedUrl)
{
	FailCount1++;
	if (FailCount1 > 3)
	{
		return;
	}
	GetWorldTimerManager().SetTimer(FailLoadVideo1Handle, this, &AJS_Screen::VideoSourceLoad1, 3.f, false);
}

void AJS_Screen::OnSucceedLoadVideo1(FString Url)
{
	FailCount1 = 0;
}

void AJS_Screen::VideoSourceLoad1()
{
	MediaPlayer->OpenSource(MediaSource);
}

void AJS_Screen::OnFailedLoadVideo2(FString FailedUrl)
{
	FailCount2++;
	if (FailCount2 > 3)
	{
		return;
	}
	GetWorldTimerManager().SetTimer(FailLoadVideo2Handle, this, &AJS_Screen::VideoSourceLoad2, 3.f, false);
}

void AJS_Screen::OnSucceedLoadVideo2(FString Url)
{
	FailCount2 = 0;
}

void AJS_Screen::VideoSourceLoad2()
{
	MediaPlayer2->OpenSource(MediaSource2);
}

void AJS_Screen::RequestSummaryVOD(int32 Time)
{
	if (!MediaPlayer || !NetComp) 
	{
		return;
	}
	// 현재 MediaPlayer의 재생 시간을 가져와
	int32 CurrentTime = MediaPlayer->GetTime().GetTotalSeconds();

	int32 Hour = CurrentTime / 3600;
	int32 Minute = (CurrentTime % 3600) / 60;
	int32 Second = CurrentTime % 60;

	FString EndTime = FString::Printf(TEXT("%02d:%02d:%02d"), Hour, Minute, Second);

	// Time(분)만큼 땡기고 
	int32 AdjustTime = CurrentTime - (Time * 60);

	// 만약 Time이 0보다 작다면 00:00:00으로
	if (AdjustTime < 0)
	{
		AdjustTime = 0;
	}

	// 00:00:00(시:분:초) 형태의 문자열로 만들기 EX) 01:05:00이라면 01:04:00으로
	Hour = AdjustTime / 3600;
	Minute = (AdjustTime % 3600) / 60;
	Second = AdjustTime % 60;

	FString StartTime = FString::Printf(TEXT("%02d:%02d:%02d"), Hour, Minute, Second);
	// 해서 AI Server에 요청 보내기
	// 여기서 해야할 건, Time을 땡기고 숫자를 문자열로 바꿔서 NetComp의 함수를 호출
	NetComp->SendSummaryRequestVOD(StartTime, EndTime);
}

void AJS_Screen::PlayMedia(const FString& VideoURL)
{
	RepVideoURL = VideoURL;
	MultiCastPlayMedia(VideoURL);
}

void AJS_Screen::PlayVOD(const FString& VideoURL)
{
	MulticastPlayVOD(VideoURL);
}

void AJS_Screen::MulticastPlayVOD_Implementation(const FString& VideoURL)
{
	// 만약 재생 중인 미디어 플레이어가 있다면 전부 멈추기
	MediaPlayer->PlayOnOpen = true;
	// Media를 재생
	// MediaSource의 URL 설정
	MediaTexture->SetMediaPlayer(MediaPlayer);
	if (MediaSource)
	{
		PRINTLOG(TEXT("%s"), *NetComp->VideoURL);
		MediaSource->StreamUrl = VideoURL;
		int32 Index;
		if (VideoURL.FindLastChar('/', Index))
		{
			NetComp->CurrentVODFileName = VideoURL.RightChop(Index+1);
		}
		// 미디어 재생 시작
		if (MediaPlayer)
		{
			MediaPlayer->OpenSource(MediaSource);
			bUsingFirstPlayer = true;
		}
	}
}

void AJS_Screen::PlayVideoRepURL(const FString& VideoURL)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	// 만약 재생 중인 미디어 플레이어가 있다면 전부 멈추기
	MediaPlayer->PlayOnOpen = true;
	GetWorldTimerManager().ClearTimer(FailLoadVideo1Handle);
	GetWorldTimerManager().ClearTimer(FailLoadVideo2Handle);
	FailCount2 = 0;
	FailCount1 = 0;

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
			MediaPlayer->Play();
			bUsingFirstPlayer = true;
		}
	}
	if (MediaSource2)
	{
		NetComp->SetVideoURL();
		MediaSource2->StreamUrl = NetComp->VideoURL + TEXT(".mp4");
		if (MediaPlayer2)
		{
			PRINTLOG(TEXT("%d"), MediaPlayer2->OpenSource(MediaSource2));
		}
	}
}

void AJS_Screen::MultiCastPlayMedia_Implementation(const FString& VideoURL)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	
	// 만약 재생 중인 미디어 플레이어가 있다면 전부 멈추기
	MediaPlayer->PlayOnOpen = true;
	GetWorldTimerManager().ClearTimer(FailLoadVideo1Handle);
	GetWorldTimerManager().ClearTimer(FailLoadVideo2Handle);
	FailCount2 = 0;
	FailCount1 = 0;
	
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
			MediaPlayer->Play();
			bUsingFirstPlayer = true;
		}
	}
	if (MediaSource2)
	{
		NetComp->SetVideoURL();
		MediaSource2->StreamUrl = NetComp->VideoURL + TEXT(".mp4");
		if (MediaPlayer2)
		{
			PRINTLOG(TEXT("%d"),MediaPlayer2->OpenSource(MediaSource2));
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

FVedioInfo::FVedioInfo(bool blive, FString time, FString title, FString owner, FString streamURL, UTexture2D* thumbnail, FString category)
{
	bLive = blive;
	Time = time;
	Title = title;
	Owner = owner;
	Thumbnail = thumbnail;
	StreamURL =streamURL;
	Category = category;
}
