// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_TotoActor.generated.h"

UCLASS()
class METACHEERINGROOM_API AJS_TotoActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_TotoActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// MakeUI
	TSubclassOf<class UToToMakeWidget> TotoMakeWidgetFactory;
	class UToToMakeWidget* TotoMakeWidget;

	// ToToUI
	TSubclassOf<class UJS_ToToWidget> ToToWidgetFactory;
	class UJS_ToToWidget* ToToWidget;

	FString TotoName;
	FString Select1;
	FString Select2;

	// 전체 포인트
	int32 TotalSelect1 = 0;
	int32 TotalSelect2 = 0;
	// 전체 참여자
	int32 TotalBettor1 = 0;
	int32 TotalBettor2 = 0;
	// 전체 배당
	float TotalOdds1 = 0.f;
	float TotalOdds2 = 0.f;
	// 내 포인트
	int32 MyPoint;
	// 내 선택
	int32 MySelect = -1;
	// 멀티플레이를 고려한 토토 만들기

	// 1. 방장이 토토를 만들기
	UFUNCTION()
	void MakeToto(FString totoName, FString select1, FString select2);
	// 2. 전체 유저에게 토토 시작 알림이 뜨면서, 토토 UI의 정보가 갱신됨
	UFUNCTION(NetMulticast,Reliable)
	void MulticastSetToToUI(const FString& totoName, const FString& select1, const FString& select2, 
	int32 totalSelect1 = 0,int32 totalSelect2 = 0, int32 totalBettor1 = 0, int32 totalBettor2 = 0, float totalOdds1 = 1.f, float totalOdds2 = 1.f);
	UFUNCTION()
	void AlarmToto(const FString& AlarmText);
	
	// 3. 유저가 토토를 걸 수 있음
	// 3-1. 유저가 토토를 걸면 방장의 토탈 Select가 증가하고, 각각의 개인의 MyPoint가 증가함
	// 3-2. 서버는 유저가 건 토토의 이름, 양을 Map을 이용해 가지고 있는다.
	// 3-3. 유저의 거는 포인트의 양에 따라 모든 유저의 UI가 변화해야함
	UFUNCTION()
	void BettingToto(int32 point, int32 select);

	UFUNCTION(Server, Reliable)
	void ServerBettingToto(int32 point, int32 select);


	// 4. 방장이 결과를 설정
	// 4-1. 맞춘 유저는 자신이 걸었던 돈을 배당에 맞게 획득함
	// 4-2. 틀린 유저는 돈을 잃음

	// 끗
};
