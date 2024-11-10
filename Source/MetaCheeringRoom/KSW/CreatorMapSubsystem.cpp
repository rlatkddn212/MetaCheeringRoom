// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorMapSubsystem.h"
#include "JsonObjectConverter.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "CreatorObject/SW_CreatorObject.h"
#include "CreatorStorageSubsystem.h"
#include "Engine/EngineTypes.h"
#include "EngineUtils.h"
#include "CreatorFBXSubsystem.h"

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

void UCreatorMapSubsystem::InitMap()
{
    CreatorMap.Objects.Empty();
	UniqueCreatorItemId = 1;

    // 월드에 있는 ASW_CreatorObject 모두 제거
    for (TActorIterator<ASW_CreatorObject> It(GetWorld()); It; ++It)
	{
		ASW_CreatorObject* CreatorObject = *It;
		CreatorObject->Destroy();
	}
}

void UCreatorMapSubsystem::SetMapName(const FString& Name)
{
    CreatorMap.MapName = Name;
}

FString UCreatorMapSubsystem::GetMapName()
{
    return CreatorMap.MapName;
}

FString UCreatorMapSubsystem::SaveCreatorMapToJson()
{
    FString JsonString = SerializeCreatorMapToJson(CreatorMap);

    return JsonString;
}

void UCreatorMapSubsystem::SetupJson(FString JsonString)
{
    LoadJsonStr = JsonString;
}

bool UCreatorMapSubsystem::LoadMap()
{
	UCreatorFBXSubsystem* fbxSubsystem = GetGameInstance()->GetSubsystem<UCreatorFBXSubsystem>();
	fbxSubsystem->bIsLoading = false;
    InitMap();
    FCreatorMap LoadedMap = DeserializeJsonToCreatorMap(LoadJsonStr);
    return true;
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
    JsonObject->SetArrayField(TEXT("rootObjects"), JsonRootArray);

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
                ASW_CreatorObject* ChildObject = DeserializeCreatorObject(JsonValue->AsObject());
                if (ChildObject)
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
        return JsonObject;  // 빈 객체 반환
    }

    // CreatorObject의 이름과 변환 정보를 JSON에 직렬화
    JsonObject->SetStringField(TEXT("ObjectName"), CreatorObject->GetName());

    JsonObject->SetNumberField(TEXT("CreatorObjectType"), CreatorObject->CreatorObjectType);
    JsonObject->SetNumberField(TEXT("CreatorObjectId"), CreatorObject->CreatorObjectId);
    JsonObject->SetNumberField(TEXT("CreatorObjectUId"), CreatorObject->CreatorObjectUId);

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

    CreatorObject->RecordJsonAdditionalInfo(JsonObject);
    // 자식 객체 배열 직렬화

    TArray<AActor*> AttachedActors;
    CreatorObject->GetAttachedActors(AttachedActors);
   TArray<TSharedPtr<FJsonValue>> JsonChildren;
    for (const AActor* Child : AttachedActors)
    {
        const ASW_CreatorObject* NewCreatorObject = Cast<ASW_CreatorObject>(Child); 
		if (NewCreatorObject != nullptr)
        {
            JsonChildren.Add(MakeShareable(new FJsonValueObject(SerializeCreatorObject(NewCreatorObject))));
        }
    }

    JsonObject->SetArrayField(TEXT("Objects"), JsonChildren);

    return JsonObject;
}

ASW_CreatorObject* UCreatorMapSubsystem::DeserializeCreatorObject(const TSharedPtr<FJsonObject>& JsonObject)
{
    if (!JsonObject.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid JSON Object provided."));
        return nullptr;
    }
    
    int32 CreatorObjectType = 1;
    int32 CreatorObjectId = 1;
    
    JsonObject->TryGetNumberField(TEXT("CreatorObjectType"), CreatorObjectType);
    JsonObject->TryGetNumberField(TEXT("CreatorObjectId"), CreatorObjectId);
    UCreatorStorageSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorStorageSubsystem>();
    TMap<int32, FCreatorObjectData*> CreatorObjectsStruct = system->GetCreatorObjects(CreatorObjectType);
    if (!CreatorObjectsStruct.Contains(CreatorObjectId))
	{
        UE_LOG(LogTemp, Warning, TEXT("Invalid CreatorObject provided."));
		return nullptr;
	}

    // CreatorObject를 생성
    ASW_CreatorObject* CreatorObject = CreateObject(CreatorObjectsStruct[CreatorObjectId]);

    CreatorObject->CreatorObjectId = CreatorObjectId;
    CreatorObject->CreatorObjectType = CreatorObjectType;

    JsonObject->TryGetNumberField(TEXT("CreatorObjectUId"), CreatorObject->CreatorObjectUId);
    UniqueCreatorItemId = FMath::Max(UniqueCreatorItemId, CreatorObject->CreatorObjectUId + 1);

    // JSON에서 이름과 변환 정보를 가져오기
    //CreatorObject->SetActorLabel(JsonObject->GetStringField(TEXT("ObjectName")));

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
    CreatorObject->SetActorTransform(transform);

    CreatorObject->SetupJsonAdditionalInfo(JsonObject);

    // 자식 객체 배열 역직렬화
    const TArray<TSharedPtr<FJsonValue>>* JsonChildren;
    if (JsonObject->TryGetArrayField(TEXT("Objects"), JsonChildren))
    {
        for (const TSharedPtr<FJsonValue>& JsonValue : *JsonChildren)
        {
            ASW_CreatorObject* ChildObject =  DeserializeCreatorObject(JsonValue->AsObject());
            //AttachObject(CreatorObject, ChildObject);
            if (ChildObject)
                AddObject(ChildObject, CreatorObject);
        }
    }

    return CreatorObject;
}


ASW_CreatorObject* UCreatorMapSubsystem::CopyObjectRecursive(ASW_CreatorObject* SourceObject)
{
    UCreatorStorageSubsystem* system = GetGameInstance()->GetSubsystem<UCreatorStorageSubsystem>();
	int32 CreatorObjectType = SourceObject->CreatorObjectType;
	int32 CreatorObjectId = SourceObject->CreatorObjectId;

    TMap<int32, FCreatorObjectData*> CreatorObjectsStruct = system->GetCreatorObjects(CreatorObjectType);
    if (!CreatorObjectsStruct.Contains(CreatorObjectId))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid CreatorObject provided."));
        return nullptr;
    }

    // CreatorObject를 생성
    ASW_CreatorObject* CreatorObject = CreateObject(CreatorObjectsStruct[CreatorObjectId]);

    CreatorObject->CreatorObjectId = CreatorObjectId;
    CreatorObject->CreatorObjectType = CreatorObjectType;

    CreatorObject->SetActorTransform(SourceObject->GetActorTransform());

    TArray<AActor*> ChildActors;
    SourceObject->GetAttachedActors(ChildActors);
    for (AActor* ChildActor : ChildActors)
    {
        ASW_CreatorObject* ChildCreatorObject = Cast<ASW_CreatorObject>(ChildActor);
        if (ChildCreatorObject != nullptr)
        {
			ASW_CreatorObject* NewChildObject = CopyObjectRecursive(ChildCreatorObject);
			if (NewChildObject != nullptr)
			{
                AddObject(NewChildObject, CreatorObject);
			}
        }
    }

	return CreatorObject;
}

ASW_CreatorObject* UCreatorMapSubsystem::CopyObject(ASW_CreatorObject* Object)
{
    // 부모를 가져온다.
    ASW_CreatorObject* ParentObject = FindParentObject(Object);

    // 복사본을 생성한다.
    ASW_CreatorObject* NewObject = CopyObjectRecursive(Object);
	if (NewObject != nullptr)
	{
        AddObject(NewObject, ParentObject);
    }

	return NewObject;
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
    CreatingObject->CreatorObjectType = ObjectData->CObjectType;
    CreatingObject->CreatorObjectId = ObjectData->CObjectId;

	return CreatingObject;
}

void UCreatorMapSubsystem::AddObject(ASW_CreatorObject* CreatingObject, ASW_CreatorObject* ParentObject /*= nullptr*/)
{
    int32 CreatorItemId = UniqueCreatorItemId++;
    CreatingObject->CreatorObjectUId = CreatorItemId;
    CreatorItemMap.Add(CreatorItemId, CreatingObject);

    CreatingObject->SetFileName(CreatingObject->CreatingObjectData->ItemName);

	if (ParentObject != nullptr)
	{
        // 부모 액터에 추가
        AttachObject(ParentObject, CreatingObject);
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

		CreatorItemMap.Remove(Object->CreatorObjectId);
		Object->Destroy();
	}
}

void UCreatorMapSubsystem::RemoveActorRecursive(AActor* Actor)
{
    if (Actor)
    {
        TArray<AActor*> AttachedActors;
        Actor->GetAttachedActors(AttachedActors);
        for (AActor* AttachedActor : AttachedActors)
        {
            RemoveActorRecursive(AttachedActor);
        }
    }

    Actor->Destroy();
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
                else
                {
                    // 재귀적으로 자식 액터까지 삭제
                    RemoveActorRecursive(AttachedActor);
                }
			}
		}

		CreatorItemMap.Remove(Object->CreatorObjectId);
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

ASW_CreatorObject* UCreatorMapSubsystem::GetParentCreatorObject(AActor* Object)
{
	// 부모가 CreatorObject인지 확인
    AActor* ParentActor = Object->GetAttachParentActor();
    if (ParentActor == nullptr)
    {
		return nullptr;
    }

	ASW_CreatorObject* ParentObject = Cast<ASW_CreatorObject>(ParentActor);
	if (ParentObject != nullptr)
	{
		return ParentObject;
	}

    return GetParentCreatorObject(ParentActor);
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
