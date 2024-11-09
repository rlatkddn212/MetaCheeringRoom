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

	/** JSON ����ȭ/������ȭ ���� �Լ� */
	TSharedPtr<FJsonObject> SerializeCreatorObject(const ASW_CreatorObject* CreatorObject);
	ASW_CreatorObject* DeserializeCreatorObject(const TSharedPtr<FJsonObject>& JsonObject);

	FCreatorMap CreatorMap;

	// ���ο� ������Ʈ ����
	ASW_CreatorObject* CreateObject(struct FCreatorObjectData* ObjectData);

	// ���� ���忡 �߰��Ѵ�. (UI���� ��� �ӽ÷� �����ϴ� ��찡 �־ World�� �߰��ϴ� �Լ��� ���� ����)
	void AddObject(ASW_CreatorObject* CreatingObject, ASW_CreatorObject* ParentObject = nullptr);

	// ������Ʈ ���� ���������
	void RemoveObject(ASW_CreatorObject* Object, bool isRecursive = false);


	// ������Ʈ �߰�
	void AttachObject(ASW_CreatorObject* ParentObject, ASW_CreatorObject* ChildObject);

	void DetechObject(ASW_CreatorObject* ParentObject, ASW_CreatorObject* ChildObject);
	
	ASW_CreatorObject* CopyObjectRecursive(ASW_CreatorObject* SourceObject);
	ASW_CreatorObject* CopyObject(ASW_CreatorObject* SourceObject);

	// �θ�ã��
	ASW_CreatorObject* FindParentObject(ASW_CreatorObject* ChildObject);

	ASW_CreatorObject* GetParentCreatorObject(AActor* Object);

	// �ڽ����� Ȯ��
	bool IsChildObject(ASW_CreatorObject* ParentObject, ASW_CreatorObject* ChildObject);

	FString LoadJsonStr;

	UPROPERTY()
	TMap<int32, ASW_CreatorObject*> CreatorItemMap;
	
	int32 UniqueCreatorItemId;
};
