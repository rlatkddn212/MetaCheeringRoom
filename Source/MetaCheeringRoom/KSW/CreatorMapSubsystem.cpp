// Fill out your copyright notice in the Description page of Project Settings.


#include "KSW/CreatorMapSubsystem.h"
#include "JsonObjectConverter.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"


void UCreatorMapSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("CreatorMapSubSystem initialized"));
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
    for (const TSharedPtr<FCreatorObject> RootObject : Map.Objects)
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

TSharedPtr<FJsonObject> UCreatorMapSubsystem::SerializeCreatorObject(const TSharedPtr<FCreatorObject>& CreatorObject)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

    if (!CreatorObject.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid CreatorObject provided."));
        return JsonObject;  // 빈 객체 반환
    }
    // CreatorObject의 이름과 변환 정보를 JSON에 직렬화
    JsonObject->SetStringField(TEXT("ObjectName"), CreatorObject->ObjectName);

    // 변환 정보를 JSON 객체로 직렬화
    JsonObject->SetNumberField(TEXT("TranslationX"), CreatorObject->ObjectTransform.GetLocation().X);
    JsonObject->SetNumberField(TEXT("TranslationY"), CreatorObject->ObjectTransform.GetLocation().Y);
    JsonObject->SetNumberField(TEXT("TranslationZ"), CreatorObject->ObjectTransform.GetLocation().Z);

    // Rotation (Quaternion)
    JsonObject->SetNumberField(TEXT("RotationX"), CreatorObject->ObjectTransform.GetRotation().X);
    JsonObject->SetNumberField(TEXT("RotationY"), CreatorObject->ObjectTransform.GetRotation().Y);
    JsonObject->SetNumberField(TEXT("RotationZ"), CreatorObject->ObjectTransform.GetRotation().Z);
    JsonObject->SetNumberField(TEXT("RotationW"), CreatorObject->ObjectTransform.GetRotation().W);

    // Scale
    JsonObject->SetNumberField(TEXT("ScaleX"), CreatorObject->ObjectTransform.GetScale3D().X);
    JsonObject->SetNumberField(TEXT("ScaleY"), CreatorObject->ObjectTransform.GetScale3D().Y);
    JsonObject->SetNumberField(TEXT("ScaleZ"), CreatorObject->ObjectTransform.GetScale3D().Z);
    // 자식 객체 배열 직렬화
    TArray<TSharedPtr<FJsonValue>> JsonChildren;
    for (const TSharedPtr<FCreatorObject>& Child : CreatorObject->Objects)
    {
        JsonChildren.Add(MakeShareable(new FJsonValueObject(SerializeCreatorObject(Child))));
    }
    JsonObject->SetArrayField(TEXT("Objects"), JsonChildren);

    return JsonObject;
}

TSharedPtr<FCreatorObject> UCreatorMapSubsystem::DeserializeCreatorObject(const TSharedPtr<FJsonObject>& JsonObject)
{
    if (!JsonObject.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid JSON Object provided."));
        return nullptr;  // null 반환
    }

    // CreatorObject를 생성
    TSharedPtr<FCreatorObject> CreatorObject = MakeShareable(new FCreatorObject);

    // JSON에서 이름과 변환 정보를 가져오기
    CreatorObject->ObjectName = JsonObject->GetStringField(TEXT("ObjectName"));

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

    CreatorObject->ObjectTransform = FTransform(Rotation, Translation, Scale);

    // 자식 객체 배열 역직렬화
    const TArray<TSharedPtr<FJsonValue>>* JsonChildren;
    if (JsonObject->TryGetArrayField(TEXT("Objects"), JsonChildren))
    {
        for (const TSharedPtr<FJsonValue>& JsonValue : *JsonChildren)
        {
            CreatorObject->Objects.Add(DeserializeCreatorObject(JsonValue->AsObject()));
        }
    }

    return CreatorObject;
}

TSharedPtr<FCreatorObject> UCreatorMapSubsystem::AddObject(const FString& ObjectName, const FTransform& ObjectTransform, TSharedPtr<FCreatorObject> ParentObject /*= nullptr*/)
{
	TSharedPtr<FCreatorObject> NewObject = MakeShareable(new FCreatorObject);
	NewObject->ObjectName = ObjectName;
	NewObject->ObjectTransform = ObjectTransform;

	if (ParentObject.IsValid())
	{
		ParentObject->Objects.Add(NewObject);
	}
	else
	{
		CreatorMap.Objects.Add(NewObject);
	}

	return NewObject;
}

void UCreatorMapSubsystem::RemoveObject(TSharedPtr<FCreatorObject> Object)
{
	if (Object.IsValid())
	{
		if (CreatorMap.Objects.Contains(Object))
		{
			CreatorMap.Objects.Remove(Object);
		}
		else
		{
			// 재귀적으로 돌면서 부모를 찾아서 삭제한다.
		}
	}
}

void UCreatorMapSubsystem::MoveObject(TSharedPtr<FCreatorObject> Object, const FVector& NewLocation)
{

}

void UCreatorMapSubsystem::RotateObject(TSharedPtr<FCreatorObject> Object, const FRotator& NewRotation)
{

}

void UCreatorMapSubsystem::ScaleObject(TSharedPtr<FCreatorObject> Object, const FVector& NewScale)
{

}

void UCreatorMapSubsystem::RenameObject(TSharedPtr<FCreatorObject> Object, const FString& NewName)
{

}

void UCreatorMapSubsystem::AddChildObject(TSharedPtr<FCreatorObject> ParentObject, TSharedPtr<FCreatorObject> ChildObject)
{
    
}

void UCreatorMapSubsystem::RemoveChildObject(TSharedPtr<FCreatorObject> ParentObject, TSharedPtr<FCreatorObject> ChildObject)
{


}

TSharedPtr<FCreatorObject> UCreatorMapSubsystem::FindObject(const FString& ObjectName)
{
	// 재귀적으로 돌면서 찾는다.
    
	return nullptr;
}


TSharedPtr<FCreatorObject> UCreatorMapSubsystem::FindParentObject(TSharedPtr<FCreatorObject> ChildObject)
{
    // 재귀적으로 돌면서 부모를 찾는다.

	return nullptr;
}


// 구조체 함수
void FCreatorObject::RemoveObject(TSharedPtr<FCreatorObject> Object)
{
    
}

void FCreatorMap::RemoveObject(TSharedPtr<FCreatorObject> Object)
{

}
