// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KSW/CreatorObject/SW_CreatorObject.h"
#include "SW_CreatorFBX.generated.h"

/**
 * 
 */
UCLASS()
class METACHEERINGROOM_API ASW_CreatorFBX : public ASW_CreatorObject
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ASW_CreatorFBX();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnSelected(bool isSelected) override;

	void SelectChildActor(AActor* actor, bool isSelected);

	virtual void SetFileName(const FString& FileName);

	virtual void RecordJsonAdditionalInfo(TSharedPtr<FJsonObject>& RecordJsonObject) const override;
	virtual void SetupJsonAdditionalInfo(const TSharedPtr<FJsonObject>& SetupJsonObject) override;

	UPROPERTY(ReplicatedUsing = OnRep_FBXFileName)
	FString FBXFileName;

	UFUNCTION()
	void OnRep_FBXFileName();

	UFUNCTION()
	void OpenFBXFile();
	FTimerHandle TimerHandle;
	//GetLifetimeReplicatedProps(LifeTime);
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;


	UFUNCTION()
	void HandleProgress(const FString& FilePath, float Progress);
	UFUNCTION()
	void HandleFinish(const FString& FilePath, AActor* ImportedActor);

	class URLFProgress* GetProgressTracker() const { return ProgressTracker; }

	class URLFProgress* ProgressTracker;

	UPROPERTY(EditAnywhere, Category = "FBX")
	class USW_FBXLoadProgressWidget* LoadProgressWidget;

	UPROPERTY(EditAnywhere, Category = "FBX")
	class UWidgetComponent* LoadProgressWidgetComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Mesh;

	float ProgressValue;
	bool isFileLoaded;
	bool isComplete;
};
