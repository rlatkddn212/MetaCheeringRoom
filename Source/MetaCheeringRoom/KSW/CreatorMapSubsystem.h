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
	/** JSON ����ȭ/������ȭ ���� �Լ� */
	TSharedPtr<FJsonObject> SerializeCreatorObject(const TSharedPtr<FCreatorObject>& CreatorObject);
	TSharedPtr<FCreatorObject> DeserializeCreatorObject(const TSharedPtr<FJsonObject>& JsonObject);

	FCreatorMap CreatorMap;

	// ũ�������� �� ���

	// ���ο� ������Ʈ �߰�
	TSharedPtr<FCreatorObject> AddObject(const FString& ObjectName, const FTransform& ObjectTransform, TSharedPtr<FCreatorObject> ParentObject = nullptr);

	// ������Ʈ ����
	void RemoveObject(TSharedPtr<FCreatorObject> Object);

	// ������Ʈ �̵�
	void MoveObject(TSharedPtr<FCreatorObject> Object, const FVector& NewLocation);
	
	// ȸ��
	void RotateObject(TSharedPtr<FCreatorObject> Object, const FRotator& NewRotation);

	// ������
	void ScaleObject(TSharedPtr<FCreatorObject> Object, const FVector& NewScale);

	// ������Ʈ �̸� ����
	void RenameObject(TSharedPtr<FCreatorObject> Object, const FString& NewName);

	// ������Ʈ �߰�
	void AddChildObject(TSharedPtr<FCreatorObject> ParentObject, TSharedPtr<FCreatorObject> ChildObject);

	// ������Ʈ ����
	void RemoveChildObject(TSharedPtr<FCreatorObject> ParentObject, TSharedPtr<FCreatorObject> ChildObject);

	// ������Ʈ ã��
	TSharedPtr<FCreatorObject> FindObject(const FString& ObjectName);

	// �θ�ã��
	TSharedPtr<FCreatorObject> FindParentObject(TSharedPtr<FCreatorObject> ChildObject);
};
