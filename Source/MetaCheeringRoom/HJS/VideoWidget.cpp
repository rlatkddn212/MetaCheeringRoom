// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/VideoWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "VideoList.h"
#include "VideoNode.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"
#include "../SHK/HG_Player.h"

void UVideoWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_CategoryLive->OnClicked.AddDynamic(this, &UVideoWidget::SettingLive);
	BTN_CategoryVOD->OnClicked.AddDynamic(this, &UVideoWidget::SettingVOD);
	BTN_Quit->OnClicked.AddDynamic(this, &UVideoWidget::OnClickQuitBtn);
	SettingLive();
}

void UVideoWidget::OnClickQuitBtn()
{
	SetVisibility(ESlateVisibility::Hidden);
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	PC->SetShowMouseCursor(false);
	PC->SetInputMode(FInputModeGameOnly());
	AHG_Player* Player = Cast<AHG_Player>(PC->GetCharacter());
	if (Player)
	{
		Player->Direction = FVector::ZeroVector;
		Player->bCanMove = true;
	}
}

void UVideoWidget::SettingLive()
{
	SB_Live->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SB_VOD->SetVisibility(ESlateVisibility::Hidden);
}

void UVideoWidget::SettingVOD()
{
	SB_VOD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SB_Live->SetVisibility(ESlateVisibility::Hidden);
}

void UVideoWidget::SettingLiveInfo(TArray<FVedioInfo>& LiveInfoList)
{
	SB_Live->ClearChildren();
	int32 count = 0;

	// 복잡한데... 그냥 두 번 돌면서 따로 추가하자
	for (int32 i = 0; i < LiveInfoList.Num()/4+1; i++)
	{
		UVideoList* VideoList = CreateWidget<UVideoList>(this,VideoListFactory);
		VideoList->NativeConstruct();
		VideoList->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		// 인포 리스트에 있는 정보를 받아와서
		for (int32 j = 0; j < 4; j++)
		{
			// 4개 단위로 쪼개서 비디오리스트에 넣고
			if (count >= LiveInfoList.Num())
			{
				break;
			}
			if (LiveInfoList[count].bLive == false)
			{
				count++;
				j--;
				continue;
			}
			// Node를 생성해
			UVideoNode* VideoNode = CreateWidget<UVideoNode>(VideoList, VideoNodeFactory);
			VideoNode->SetVisibility(ESlateVisibility::Visible);
			if (VideoNode)
			{
				// Node에 세팅해주기
				FString Title = RemoveEmojis(LiveInfoList[count].Title);
				if (LiveInfoList[count].Category == TEXT("Chzzk"))
				{
					Title = Title.LeftChop(10);
				}
				if (LiveInfoList[count].Title.Len() > 26)
				{
					Title = Title.Left(26);
					Title = Title + TEXT("...");
				}
				
				VideoNode->TEXT_Title->SetText(FText::FromString(Title));
				VideoNode->TEXT_Owner->SetText(FText::FromString(LiveInfoList[count].Owner));
				VideoNode->TEXT_Time->SetText(FText::FromString(LiveInfoList[count].Time));
				VideoNode->IMG_Thumbnail->SetBrushFromTexture(LiveInfoList[count].Thumbnail);
				VideoNode->PlayURL = LiveInfoList[count].StreamURL;
				VideoList->NodeArr[j]->AddChild(VideoNode);
			}
			count++;
		}
		// 비디오 리스트를 스크롤바에 추가하기
		SB_Live->AddChild(VideoList);

		if (count >= LiveInfoList.Num())
		{
			break;
		}
	}

	SB_VOD->ClearChildren();
	count = 0;
	// 복잡한데... 그냥 두 번 돌면서 따로 추가하자
	for (int32 i = 0; i < LiveInfoList.Num() / 4 + 1; i++)
	{
		UVideoList* VideoList = CreateWidget<UVideoList>(this, VideoListFactory);
		VideoList->NativeConstruct();
		VideoList->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		// 인포 리스트에 있는 정보를 받아와서
		for (int32 j = 0; j < 4; j++)
		{
			// 4개 단위로 쪼개서 비디오리스트에 넣고
			if (count >= LiveInfoList.Num())
			{
				break;
			}
			if (LiveInfoList[count].bLive == true)
			{
				count++;
				j--;
				continue;
			}
			// Node를 생성해
			UVideoNode* VideoNode = CreateWidget<UVideoNode>(VideoList, VideoNodeFactory);
			VideoNode->SetVisibility(ESlateVisibility::Visible);
			if (VideoNode)
			{
				// Node에 세팅해주기
				FString Title = RemoveEmojis(LiveInfoList[count].Title);
				if (LiveInfoList[count].Category == TEXT("Chzzk"))
				{
					Title = Title.LeftChop(10);
				}
				if (LiveInfoList[count].Title.Len() > 26)
				{
					Title = Title.Left(26);
					Title = Title + TEXT("...");
				}

				VideoNode->TEXT_Title->SetText(FText::FromString(Title));
				VideoNode->TEXT_Owner->SetText(FText::FromString(LiveInfoList[count].Owner));
				VideoNode->TEXT_Time->SetText(FText::FromString(LiveInfoList[count].Time));
				VideoNode->IMG_Thumbnail->SetBrushFromTexture(LiveInfoList[count].Thumbnail);
				VideoNode->PlayURL = LiveInfoList[count].StreamURL;
				VideoNode->bLive = false;
				VideoList->NodeArr[j]->AddChild(VideoNode);
			}
			count++;
		}
		// 비디오 리스트를 스크롤바에 추가하기
		SB_VOD->AddChild(VideoList);

		if (count >= LiveInfoList.Num())
		{
			break;
		}
	}

}

bool UVideoWidget::IsEmoji(char32_t Codepoint)
{
	if (Codepoint >= 0xAC00 && Codepoint <= 0xD7A3) {
		return false; // 한글 음절이므로 이모지가 아님
	}
	// 이모지 유니코드 범위를 확인
	return (Codepoint >= 0x1F600 && Codepoint <= 0x1F64F) || // 얼굴 이모지
		(Codepoint >= 0x1F300 && Codepoint <= 0x1F5FF) || // 기타 심볼 이모지
		(Codepoint >= 0x1F680 && Codepoint <= 0x1F6FF) || // 운송 수단 이모지
		(Codepoint >= 0x1F1E0 && Codepoint <= 0x1F1FF) || // 국기 이모지
		(Codepoint >= 0x2702 && Codepoint <= 0x27B0) ||  // 다양한 기호 이모지
		(Codepoint >= 0x24C2 && Codepoint <= 0x1F251);    // 추가 기호 및 문장 부호
}

FString UVideoWidget::RemoveEmojis(const FString& Input)
{
    FString Result;
    const int32 Len = Input.Len();

    for (int32 i = 0; i < Len;) {
        char32_t Codepoint;

        // UTF-16 서러게이트 페어 처리
        if (IsHighSurrogate(Input[i]) && i + 1 < Len && IsLowSurrogate(Input[i + 1])) {
            // 서러게이트 페어를 디코드
            Codepoint = DecodeSurrogatePair(Input[i], Input[i + 1]);
            i += 2; // 서러게이트 페어는 두 글자로 처리
        } else {
            // 단일 유니코드 포인트 처리 (서러게이트 페어가 아닌 경우)
            Codepoint = Input[i];
            i += 1;
        }

        // 이모지가 아닌 경우에만 결과에 추가
        if (!IsEmoji(Codepoint)) {
            Result.AppendChar(static_cast<TCHAR>(Codepoint)); // 단일 유니코드 문자 그대로 추가
        }
    }

    return Result;
}

bool UVideoWidget::IsHighSurrogate(TCHAR Char)
{
	return Char >= 0xD800 && Char <= 0xDBFF;
}

bool UVideoWidget::IsLowSurrogate(TCHAR Char)
{
	return Char >= 0xDC00 && Char <= 0xDFFF;
}

char32_t UVideoWidget::DecodeSurrogatePair(TCHAR HighSurrogate, TCHAR LowSurrogate)
{
	return (((HighSurrogate - 0xD800) << 10) | (LowSurrogate - 0xDC00)) + 0x10000;
}
