// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SharedPointer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CreatorMapSubsystem.generated.h"

class ASW_CreatorObject;

//struct
USTRUCT(BlueprintType)
struct FCreatorMap
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ASW_CreatorObject*> Objects;

	FCreatorMap() : MapName(TEXT("New Map")) {}
};

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UCreatorMapSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	void RemoveObjectRecursive(ASW_CreatorObject* Object);
	void RemoveActorRecursive(AActor* Actor);

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	const FCreatorMap& GetCreatorMap() const { return CreatorMap; }
	void SetCreatorMap(const FCreatorMap& NewMap) { CreatorMap = NewMap; }
	
	void InitMap();
	void SetMapName(const FString& Name);
	FString GetMapName();

	FString SaveCreatorMapToJson();
	void SetupJson(FString JsonString);
	bool LoadMap();

	bool LoadCreatorMapFromJson(FString JsonString);

	FString SerializeCreatorMapToJson(const FCreatorMap& Map);
	FCreatorMap DeserializeJsonToCreatorMap(const FString& JsonString);

	/** JSON 직렬화/역직렬화 헬퍼 함수 */
	TSharedPtr<FJsonObject> SerializeCreatorObject(const ASW_CreatorObject* CreatorObject);
	ASW_CreatorObject* DeserializeCreatorObject(const TSharedPtr<FJsonObject>& JsonObject);

	FCreatorMap CreatorMap;

	// 새로운 오브젝트 생성
	ASW_CreatorObject* CreateObject(struct FCreatorObjectData* ObjectData);

	// 실제 월드에 추가한다. (UI에서 끌어서 임시로 생성하는 경우가 있어서 World에 추가하는 함수를 따로 만듦)
	void AddObject(ASW_CreatorObject* CreatingObject, ASW_CreatorObject* ParentObject = nullptr);

	// 오브젝트 삭제 재귀적으로
	void RemoveObject(ASW_CreatorObject* Object, bool isRecursive = false);


	// 오브젝트 추가
	void AttachObject(ASW_CreatorObject* ParentObject, ASW_CreatorObject* ChildObject);

	void DetechObject(ASW_CreatorObject* ParentObject, ASW_CreatorObject* ChildObject);
	
	ASW_CreatorObject* CopyObjectRecursive(ASW_CreatorObject* SourceObject);
	ASW_CreatorObject* CopyObject(ASW_CreatorObject* SourceObject);

	// 부모찾기
	ASW_CreatorObject* FindParentObject(ASW_CreatorObject* ChildObject);

	ASW_CreatorObject* GetParentCreatorObject(AActor* Object);

	// 자식인지 확인
	bool IsChildObject(ASW_CreatorObject* ParentObject, ASW_CreatorObject* ChildObject);

	FString LoadJsonStr;

	UPROPERTY()
	TMap<int32, ASW_CreatorObject*> CreatorItemMap;
	
	int32 UniqueCreatorItemId;
};
