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

bool UCreatorMapSubsystem::SaveCreatorMapToJson(const FString& FilePath)
{
    FString JsonString = SerializeCreatorMapToJson(CreatorMap);

    if (FFileHelper::SaveStringToFile(JsonString, *FilePath))
    {
        UE_LOG(LogTemp, Log, TEXT("CreatorMap saved to %s"), *FilePath);
        return true;
    }
    UE_LOG(LogTemp, Error, TEXT("Failed to save CreatorMap to %s"), *FilePath);
    return false;
}

bool UCreatorMapSubsystem::LoadCreatorMapFromJson(const FString& FilePath)
{
    FString JsonString;
    if (FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        CreatorMap = DeserializeJsonToCreatorMap(JsonString);
        UE_LOG(LogTemp, Log, TEXT("CreatorMap loaded from %s"), *FilePath);
        return true;
    }
    UE_LOG(LogTemp, Error, TEXT("Failed to load CreatorMap from %s"), *FilePath);
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

    // JSON 파서 생성
    TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(JsonString);
    TSharedPtr<FJsonObject> JsonObject;

    // JSON 파싱
    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        const TArray<TSharedPtr<FJsonValue>>* JsonRootArray;
        if (JsonObject->TryGetArrayField(TEXT("rootObjects"), JsonRootArray))
        {
            for (const TSharedPtr<FJsonValue>& JsonValue : *JsonRootArray)
            {
                RetCreatorMap.Objects.Add(DeserializeCreatorObject(JsonValue->AsObject()));
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
        return JsonObject;  // 빈 객체 반환
    }

    // CreatorObject의 이름과 변환 정보를 JSON에 직렬화
    JsonObject->SetStringField(TEXT("ObjectName"), CreatorObject->GetName());

    FTransform transform = CreatorObject->GetTransform();
    // 변환 정보를 JSON 객체로 직렬화
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
    // 자식 객체 배열 직렬화
   /* TArray<TSharedPtr<FJsonValue>> JsonChildren;
    for (const ASW_CreatorObject* Child : CreatorObject->GetAllChildActors())
    {
        JsonChildren.Add(MakeShareable(new FJsonValueObject(SerializeCreatorObject(Child))));
    }
    JsonObject->SetArrayField(TEXT("Objects"), JsonChildren);*/

    return JsonObject;
}

ASW_CreatorObject* UCreatorMapSubsystem::DeserializeCreatorObject(const TSharedPtr<FJsonObject>& JsonObject)
{
    if (!JsonObject.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid JSON Object provided."));
        return nullptr;  // null 반환
    }

    // CreatorObject를 생성
    //ASW_CreatorObject* CreatorObject;

    // JSON에서 이름과 변환 정보를 가져오기
    // CreatorObject->SetName = JsonObject->GetStringField(TEXT("ObjectName"));

    // 변환 정보 역직렬화
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

    // 자식 객체 배열 역직렬화
    const TArray<TSharedPtr<FJsonValue>>* JsonChildren;
    if (JsonObject->TryGetArrayField(TEXT("Objects"), JsonChildren))
    {
        for (const TSharedPtr<FJsonValue>& JsonValue : *JsonChildren)
        {
            //CreatorObject->Objects.Add(DeserializeCreatorObject(JsonValue->AsObject()));
        }
    }

    return nullptr;
}

ASW_CreatorObject* UCreatorMapSubsystem::CreateObject(FCreatorObjectData* ObjectData)
{
    FActorSpawnParameters SpawnParams;
    //SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ASW_CreatorObject* CreatingObject = GetWorld()->SpawnActor<ASW_CreatorObject>(ObjectData->ItemClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
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
        // 부모 액터에 추가
        CreatingObject->AttachToActor(ParentObject, FAttachmentTransformRules::KeepRelativeTransform);
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
