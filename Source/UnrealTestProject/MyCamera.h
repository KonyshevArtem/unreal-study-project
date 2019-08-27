// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Camera/CameraActor.h"
#include "GameFramework/Character.h"
#include "MyCamera.generated.h"

UCLASS()
class UNREALTESTPROJECT_API AMyCamera : public ACameraActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MyCamera Parameters")
		FVector CameraOffset;

	// Sets default values for this actor's properties
	AMyCamera();
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
		AActor* targetActor;

};
