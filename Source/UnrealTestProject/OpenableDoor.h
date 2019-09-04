// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Components/TimelineComponent.h"
#include "OpenableDoor.generated.h"

UCLASS()
class UNREALTESTPROJECT_API AOpenableDoor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AOpenableDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Openable door properties")
		bool IsOverlapping;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Openable door properties")
		bool IsOpened;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Openable door properties")
		UBoxComponent* OuterTrigger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Openable door properties")
		UBoxComponent* InnerTrigger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Openable door properties")
		USceneComponent* Door;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Openable door properties")
		UCurveFloat* DoorRotationZCurve;


	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

private:
	int rotationMultiplier, appliedRotationMultiplier;

	UPROPERTY()
		TMap<UBoxComponent*, int> rotationMultiplierToTrigger;
	UPROPERTY()
		UTimelineComponent* openDoorTimeline;
	UPROPERTY()
		FTimerHandle openDoorDelayHandle;

	UFUNCTION(BlueprintCallable)
		void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		                           const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
		void SetDoorRotation(float interpolatedVal);

	void SetIsOpened();
	void InitOpenDoorTimeline();
	void OpenDoor();
};
