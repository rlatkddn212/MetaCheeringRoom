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

	// delete
	// void RemoveObject(ASW_CreatorObject* Object);
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

	/** JSON ����ȭ/������ȭ ���� �Լ� */
	TSharedPtr<FJsonObject> SerializeCreatorObject(const ASW_CreatorObject* CreatorObject);
	ASW_CreatorObject* DeserializeCreatorObject(const TSharedPtr<FJsonObject>& JsonObject);

	FCreatorMap CreatorMap;

	// ���ο� ������Ʈ �߰�
	void AddObject(ASW_CreatorObject* CreatingObject, ASW_CreatorObject* ParentObject = nullptr);

	// ������Ʈ ����
	void RemoveObject(ASW_CreatorObject* Object);

	// ������Ʈ �̵�
	void MoveObject(ASW_CreatorObject* Object, const FVector& NewLocation);
	
	// ȸ��
	void RotateObject(ASW_CreatorObject* Object, const FRotator& NewRotation);

	// ������
	void ScaleObject(ASW_CreatorObject* Object, const FVector& NewScale);

	// ������Ʈ �̸� ����
	void RenameObject(ASW_CreatorObject* Object, const FString& NewName);

	// ������Ʈ �߰�
	void AddChildObject(ASW_CreatorObject* ParentObject, ASW_CreatorObject* ChildObject);

	// ������Ʈ ����
	void RemoveChildObject(ASW_CreatorObject* ParentObject, ASW_CreatorObject* ChildObject);

	// ������Ʈ ã��
	ASW_CreatorObject* FindObject(const FString& ObjectName);

	// �θ�ã��
	ASW_CreatorObject* FindParentObject(ASW_CreatorObject* ChildObject);

	UPROPERTY()
	TMap<int32, ASW_CreatorObject*> CreatorItemMap;
	
	int32 UniqueCreatorItemId;
};
