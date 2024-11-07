// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_AtkAnimInstance.h"
#include "JS_TotoActor.h"

void UJS_AtkAnimInstance::SetTotoActor(class AJS_TotoActor* NewActor)
{
	Toto = NewActor;
}

void UJS_AtkAnimInstance::AnimNotify_AtkEnd()
{
	// 소환한 액터의 애니메이션이 끝날 때 노티파이로 플레이어를 찌그러뜨리는 함수 발동 -> TotoActor의 PlayerNotify 함수임
	if (Toto)
	{
		Toto->PlayerModify();
	}
	
	// 소환한 액터는 애니메이션이 끝나고 사라짐.
	GetOwningActor()->Destroy();

}
