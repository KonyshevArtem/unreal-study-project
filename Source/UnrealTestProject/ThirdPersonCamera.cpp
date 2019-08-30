// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonCamera.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"
#include "MyUtils.h"

AThirdPersonCamera::AThirdPersonCamera()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AThirdPersonCamera::BeginPlay()
{
	Super::BeginPlay();

	AActor* parent = GetParentActor();
	if (!parent)
	{
		MyUtils::LogError("No default target on ThirdPersonCamera");
		return;
	}
	SetTarget(parent);

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if (playerController)
	{
		playerController->InputComponent->BindAxis("MouseX", this, &AThirdPersonCamera::SetMouseX);
		playerController->InputComponent->BindAxis("MouseY", this, &AThirdPersonCamera::SetMouseY);
	}
	else
	{
		MyUtils::LogError("First player controller not found");
	}
	currentRotation = GetActorRotation();
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void AThirdPersonCamera::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!target) return;
	
	currentRotation.Add(-mouseY * MouseSensitivity.Y, mouseX * MouseSensitivity.X, 0);
	currentRotation.Pitch = FMath::Clamp(currentRotation.Pitch, AngleBoundaries.X, AngleBoundaries.Y);
	const FVector direction = currentRotation.Vector();
	const FVector orbitPosition = target->GetActorLocation();
	FVector cameraPosition = orbitPosition + direction * CameraDistance;
	SetActorRotation((orbitPosition - cameraPosition).Rotation());
	const FVector cameraOffset = GetActorRightVector() * CameraOffset.X + target->GetActorUpVector() * CameraOffset.Y;
	cameraPosition += cameraOffset;

	FHitResult hitResult;
	if (GetWorld()->LineTraceSingleByChannel(hitResult, orbitPosition + FVector(0, 0, CameraOffset.Y), cameraPosition, ECC_Camera))
	{
		targetCollisionOffset = hitResult.ImpactNormal * 10;
		cameraPosition = hitResult.ImpactPoint;
	}
	else
	{
		targetCollisionOffset = FVector::ZeroVector;
	}
	currentCollisionOffset = FMath::Lerp(currentCollisionOffset, targetCollisionOffset, DeltaSeconds * 10);
	cameraPosition += currentCollisionOffset;

	SetActorLocation(FMath::Lerp(GetActorLocation(), cameraPosition, DeltaSeconds * 100));
}

void AThirdPersonCamera::SetTarget(AActor* newTarget)
{
	target = newTarget;
}
