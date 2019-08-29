// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonCamera.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"

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
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "[Error] No parent on ThirdPersonCamera");
		UE_LOG(LogTemp, Error, TEXT("No parent on ThirdPersonCamera"));
		return;
	}
	if (!parent->InputComponent)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "[Error] No input component on ThirdPersonCamera parent");
		UE_LOG(LogTemp, Error, TEXT("No input component on ThirdPersonCamera parent"));
		return;
	}
	
	parent->InputComponent->BindAxis("MouseX", this, &AThirdPersonCamera::SetMouseX);
	parent->InputComponent->BindAxis("MouseY", this, &AThirdPersonCamera::SetMouseY);
	currentRotation = GetActorRotation();
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void AThirdPersonCamera::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	currentRotation.Add(-mouseY * MouseSensitivity.Y, mouseX * MouseSensitivity.X, 0);
	currentRotation.Pitch = FMath::Clamp(currentRotation.Pitch, AngleBoundaries.X, AngleBoundaries.Y);
	const FVector direction = currentRotation.Vector();
	const FVector orbitPosition = GetParentActor()->GetActorLocation();
	FVector cameraPosition = orbitPosition + direction * CameraDistance;
	SetActorRotation((orbitPosition - cameraPosition).Rotation());
	const FVector cameraOffset = GetActorRightVector() * CameraOffset.X + GetParentActor()->GetActorUpVector() * CameraOffset.Y;
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
