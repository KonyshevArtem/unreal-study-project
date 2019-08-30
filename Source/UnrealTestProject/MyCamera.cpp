// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCamera.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "MyUtils.h"

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
		MyUtils::LogError("No parent actor on MyCamera");
	}
}

// Called every frame
void AMyCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!targetActor) return;
	
	SetActorLocation(targetActor->GetActorLocation() + CameraOffset);
	SetActorRotation((targetActor->GetActorLocation() - GetActorLocation()).Rotation());
}

