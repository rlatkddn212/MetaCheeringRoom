// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorMapSubsystem.h"
#include "JsonObjectConverter.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "SW_CreatorObject.h"
#include "CreatorStorageSubsystem.h"
#include "Engine/EngineTypes.h"

void UCreatorMapSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("CreatorMapSubSystem initialized"));
    UniqueCreatorItemId = 1;
}

void UCreatorMapSubsystem::Deinitialize()
{
	UE_LOG(LogTemp, Log, TEXT("CreatorMapSubSystem deinitialized"));
	Super::Deinitialize();
}

FString UCreatorMapSubsystem::SaveCreatorMapToJson()
{
    FString JsonString = SerializeCreatorMapToJson(CreatorMap);

    return JsonString;
}

bool UCreatorMapSubsystem::LoadCreatorMapFromJson(FString JsonString)
{
    FCreatorMap LoadedMap = DeserializeJsonToCreatorMap(JsonString);
    return false;
}

FString UCreatorMapSubsystem::SerializeCreatorMapToJson(const FCreatorMap& Map)
{
    TArray<TSharedPtr<FJsonValue>> JsonRootArray;
    for (const ASW_CreatorObject* RootObject : Map.Objects)
    {
        JsonRootArray.Add(MakeShareable(new FJsonValueObject(SerializeCreatorObject(RootObject))));
    }

    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    JsonObject->SetArrayField("rootObjects", JsonRootArray);

    FString OutputString;
    TSharedRef<TJsonWriter<TCHAR>> Writer = TJsonWriterFactory<TCHAR>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    return OutputString;
}

FCreatorMap UCreatorMapSubsystem::DeserializeJsonToCreatorMap(const FString& JsonString)
{
    FCreatorMap RetCreatorMap;

    // JSON �ļ� ����
    TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(JsonString);
    TSharedPtr<FJsonObject> JsonObject;

    // JSON �Ľ�
    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        const TArray<TSharedPtr<FJsonValue>>* JsonRootArray;
        if (JsonObject->TryGetArrayField(TEXT("rootObjects"), JsonRootArray))
        {
            for (const TSharedPtr<FJsonValue>& JsonValue : *JsonRootArray)
            {
                ASW_CreatorObject* ChildObject = DeserializeCreatorObject(JsonValue->AsObject());
                AddObject(ChildObject);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON."));
    }

    return RetCreatorMap;
}

TSharedPtr<FJsonObject> UCreatorMapSubsystem::SerializeCreatorObject(const ASW_CreatorObject* CreatorObject)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

    if (CreatorObject == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid CreatorObject provided."));
        return JsonObject;  // �� ��ü ��ȯ
    }

    // CreatorObject�� �̸��� ��ȯ ������ JSON�� ����ȭ
    JsonObject->SetStringField(TEXT("ObjectName"), CreatorObject->GetName());

    FTransform transform = CreatorObject->GetTransform();
    // ��ȯ ������ JSON ��ü�� ����ȭ
    JsonObject->SetNumberField(TEXT("TranslationX"), transform.GetLocation().X);
    JsonObject->SetNumberField(TEXT("TranslationY"), transform.GetLocation().Y);
    JsonObject->SetNumberField(TEXT("TranslationZ"), transform.GetLocation().Z);

    // Rotation (Quaternion)
    JsonObject->SetNumberField(TEXT("RotationX"), transform.GetRotation().X);
    JsonObject->SetNumberField(TEXT("RotationY"), transform.GetRotation().Y);
    JsonObject->SetNumberField(TEXT("RotationZ"), transform.GetRotation().Z);
    JsonObject->SetNumberField(TEXT("RotationW"), transform.GetRotation().W);

    // Scale
    JsonObject->SetNumberField(TEXT("ScaleX"), transform.GetScale3D().X);
    JsonObject->SetNumberField(TEXT("ScaleY"), transform.GetScale3D().Y);
    JsonObject->SetNumberField(TEXT("ScaleZ"), transform.GetScale3D().Z);
    // �ڽ� ��ü �迭 ����ȭ

    TArray<AActor*> AttachedActors;
    CreatorObject->GetAttachedActors(AttachedActors);
   TArray<TSharedPtr<FJsonValue>> JsonChildren;
    for (const AActor* Child : AttachedActors)
    {
        const ASW_CreatorObject* CreatorObject = Cast<ASW_CreatorObject>(Child); 
        JsonChildren.Add(MakeShareable(new FJsonValueObject(SerializeCreatorObject(CreatorObject))));
    }

    JsonObject->SetArrayField(TEXT("Objects"), JsonChildren);

    return JsonObject;
}

ASW_CreatorObject* UCreatorMapSubsystem::DeserializeCreatorObject(const TSharedPtr<FJsonObject>& JsonObject)
{
    if (!JsonObject.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid JSON Object provided."));
        return nullptr;  // null ��ȯ
    }

    UCreatorStorageSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorStorageSubsystem>();
    TArray<FCreatorObjectData*> CreatorObjectsStruct = system->GetCreatorObjects();
    // CreatorObject�� ����
    ASW_CreatorObject* CreatorObject = CreateObject(CreatorObjectsStruct[0]);

     // JSON���� �̸��� ��ȯ ������ ��������
    CreatorObject->SetActorLabel(JsonObject->GetStringField(TEXT("ObjectName")));

    // ��ȯ ���� ������ȭ
    FVector Translation(
        JsonObject->GetNumberField(TEXT("TranslationX")),
        JsonObject->GetNumberField(TEXT("TranslationY")),
        JsonObject->GetNumberField(TEXT("TranslationZ"))
    );

    FQuat Rotation(
        JsonObject->GetNumberField(TEXT("RotationX")),
        JsonObject->GetNumberField(TEXT("RotationY")),
        JsonObject->GetNumberField(TEXT("RotationZ")),
        JsonObject->GetNumberField(TEXT("RotationW"))
    );

    FVector Scale(
        JsonObject->GetNumberField(TEXT("ScaleX")),
        JsonObject->GetNumberField(TEXT("ScaleY")),
        JsonObject->GetNumberField(TEXT("ScaleZ"))
    );

    FTransform transform = FTransform(Rotation, Translation, Scale);
    CreatorObject->SetActorTransform(transform);

    // �ڽ� ��ü �迭 ������ȭ
    const TArray<TSharedPtr<FJsonValue>>* JsonChildren;
    if (JsonObject->TryGetArrayField(TEXT("Objects"), JsonChildren))
    {
        for (const TSharedPtr<FJsonValue>& JsonValue : *JsonChildren)
        {
            ASW_CreatorObject* ChildObject =  DeserializeCreatorObject(JsonValue->AsObject());
            //AttachObject(CreatorObject, ChildObject);
            AddObject(ChildObject, CreatorObject);
        }
    }

    return CreatorObject;
}

ASW_CreatorObject* UCreatorMapSubsystem::CreateObject(FCreatorObjectData* ObjectData)
{
    FActorSpawnParameters SpawnParams;
    //SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ASW_CreatorObject* CreatingObject = GetWorld()->SpawnActor<ASW_CreatorObject>(ObjectData->ItemClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
    CreatingObject->SetReplicates(true);
    CreatingObject->SetReplicateMovement(true);

    CreatingObject->SetActorLocation(FVector::ZeroVector);
    CreatingObject->SetActorRotation(FRotator::ZeroRotator);
    CreatingObject->CreatingObjectData = ObjectData;

	return CreatingObject;

}

void UCreatorMapSubsystem::AddObject(ASW_CreatorObject* CreatingObject, ASW_CreatorObject* ParentObject /*= nullptr*/)
{
    int32 CreatorItemId = UniqueCreatorItemId++;
    CreatingObject->CreatorItemId = CreatorItemId;
    CreatorItemMap.Add(CreatorItemId, CreatingObject);

	if (ParentObject != nullptr)
	{
        // �θ� ���Ϳ� �߰�
        AttachObject(ParentObject, CreatingObject);
        //CreatingObject->AttachToActor(ParentObject, FAttachmentTransformRules::KeepRelativeTransform);
	}
	else
	{
		CreatorMap.Objects.Add(CreatingObject);
	}
}

void UCreatorMapSubsystem::RemoveObjectRecursive(ASW_CreatorObject* Object)
{
	if (Object != nullptr)
	{
		TArray<AActor*> AttachedActors;
		Object->GetAttachedActors(AttachedActors);
		for (AActor* AttachedActor : AttachedActors)
		{
			ASW_CreatorObject* AttachedCreatorObject = Cast<ASW_CreatorObject>(AttachedActor);
			if (AttachedCreatorObject != nullptr)
			{
				RemoveObjectRecursive(AttachedCreatorObject);
			}
		}

		CreatorItemMap.Remove(Object->CreatorItemId);
		Object->Destroy();
	}
}

void UCreatorMapSubsystem::RemoveObject(ASW_CreatorObject* Object, bool isRecursive /*= false*/)
{
	if (Object != nullptr)
	{
        if (CreatorMap.Objects.Contains(Object))
        {
            CreatorMap.Objects.Remove(Object);
        }

		if (isRecursive)
		{
			TArray<AActor*> AttachedActors;
			Object->GetAttachedActors(AttachedActors);
			for (AActor* AttachedActor : AttachedActors)
			{
				ASW_CreatorObject* AttachedCreatorObject = Cast<ASW_CreatorObject>(AttachedActor);
				if (AttachedCreatorObject != nullptr)
				{
                    RemoveObjectRecursive(AttachedCreatorObject);
				}
			}
		}

		CreatorItemMap.Remove(Object->CreatorItemId);
		Object->Destroy();
	}
}

void UCreatorMapSubsystem::AttachObject(ASW_CreatorObject* ParentObject, ASW_CreatorObject* ChildObject)
{
	if (ParentObject == nullptr)
	{
		CreatorMap.Objects.Add(ChildObject);
	}
	else
	{
		ChildObject->AttachToActor(ParentObject, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void UCreatorMapSubsystem::DetechObject(ASW_CreatorObject* ParentObject, ASW_CreatorObject* ChildObject)
{
	if (ParentObject == nullptr)
	{
        if (CreatorMap.Objects.Contains(ChildObject))
        {
            CreatorMap.Objects.Remove(ChildObject);
        }
	}
    else
    {
	    ChildObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    }
}

ASW_CreatorObject* UCreatorMapSubsystem::FindParentObject(ASW_CreatorObject* ChildObject)
{
	return Cast<ASW_CreatorObject>(ChildObject->GetAttachParentActor());
}

bool UCreatorMapSubsystem::IsChildObject(ASW_CreatorObject* ParentObject, ASW_CreatorObject* ChildObject)
{
	if (ParentObject == ChildObject)
	{
		return true;
	}

    TArray<AActor*> AttachedActors;
    ParentObject->GetAttachedActors(AttachedActors);
    for (AActor* AttachedActor : AttachedActors)
	{
		ASW_CreatorObject* AttachedCreatorObject = Cast<ASW_CreatorObject>(AttachedActor);
		if (AttachedCreatorObject != nullptr)
		{
			if (IsChildObject(AttachedCreatorObject, ChildObject))
			{
				return true;
			}
		}
	}

	return false;
}
