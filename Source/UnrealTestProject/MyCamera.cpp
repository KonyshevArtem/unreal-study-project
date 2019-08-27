// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCamera.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

// Sets default values
AMyCamera::AMyCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMyCamera::BeginPlay()
{
	Super::BeginPlay();
	targetActor = GetParentActor();
	if (!targetActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString("[Error]: No parent actor on MyCamera"));
		UE_LOG(LogTemp, Error, TEXT("No parent actor on MyCamera"));
	}
}

// Called every frame
void AMyCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (targetActor)
	{
		SetActorLocation(targetActor->GetActorLocation() + CameraOffset);
		SetActorRotation((targetActor->GetActorLocation() - GetActorLocation()).Rotation());
	}
}

