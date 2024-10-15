// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SharedPointer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CreatorMapSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FCreatorObject
{
	GENERATED_BODY() 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creator")
	FString ObjectName; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creator")
	FTransform ObjectTransform;
	
    TArray<TSharedPtr<FCreatorObject>> Objects;

	// delete
	void RemoveObject(TSharedPtr<FCreatorObject> Object);
};

//struct
USTRUCT(BlueprintType)
struct FCreatorMap
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MapName;

	TArray<TSharedPtr<FCreatorObject>> Objects;

	FCreatorMap() : MapName(TEXT("New Map")) {}

	// delete
	void RemoveObject(TSharedPtr<FCreatorObject> Object);
};

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API UCreatorMapSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	const FCreatorMap& GetCreatorMap() const { return CreatorMap; }
	void SetCreatorMap(const FCreatorMap& NewMap) { CreatorMap = NewMap; }

	bool SaveCreatorMapToJson(const FString& FilePath);
	bool LoadCreatorMapFromJson(const FString& FilePath);

	FString SerializeCreatorMapToJson(const FCreatorMap& Map);
	FCreatorMap DeserializeJsonToCreatorMap(const FString& JsonString);

private:
	/** JSON 직렬화/역직렬화 헬퍼 함수 */
	TSharedPtr<FJsonObject> SerializeCreatorObject(const TSharedPtr<FCreatorObject>& CreatorObject);
	TSharedPtr<FCreatorObject> DeserializeCreatorObject(const TSharedPtr<FJsonObject>& JsonObject);

	FCreatorMap CreatorMap;

	// 크리에이터 툴 기능

	// 새로운 오브젝트 추가
	TSharedPtr<FCreatorObject> AddObject(const FString& ObjectName, const FTransform& ObjectTransform, TSharedPtr<FCreatorObject> ParentObject = nullptr);

	// 오브젝트 삭제
	void RemoveObject(TSharedPtr<FCreatorObject> Object);

	// 오브젝트 이동
	void MoveObject(TSharedPtr<FCreatorObject> Object, const FVector& NewLocation);
	
	// 회전
	void RotateObject(TSharedPtr<FCreatorObject> Object, const FRotator& NewRotation);

	// 스케일
	void ScaleObject(TSharedPtr<FCreatorObject> Object, const FVector& NewScale);

	// 오브젝트 이름 변경
	void RenameObject(TSharedPtr<FCreatorObject> Object, const FString& NewName);

	// 오브젝트 추가
	void AddChildObject(TSharedPtr<FCreatorObject> ParentObject, TSharedPtr<FCreatorObject> ChildObject);

	// 오브젝트 제거
	void RemoveChildObject(TSharedPtr<FCreatorObject> ParentObject, TSharedPtr<FCreatorObject> ChildObject);

	// 오브젝트 찾기
	TSharedPtr<FCreatorObject> FindObject(const FString& ObjectName);

	// 부모찾기
	TSharedPtr<FCreatorObject> FindParentObject(TSharedPtr<FCreatorObject> ChildObject);
};
