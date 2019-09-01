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

	ApplyRotation();
	const FVector orbitPosition = target->GetOwner()->GetActorLocation();
	const FVector cameraPosition = GetNewCameraPosition(orbitPosition);
	const FVector relativeCameraOffset = GetCameraOffset();
	const FVector collidedCameraPosition = GetCollidedCameraPosition(orbitPosition, cameraPosition + relativeCameraOffset);

	SetActorRotation(FMath::RInterpTo(GetActorRotation(), (orbitPosition - cameraPosition).Rotation(), DeltaSeconds, RotationSpeed));
	SetActorLocation(FMath::VInterpTo(GetActorLocation(), collidedCameraPosition, DeltaSeconds, PositionSpeed));
}

FVector AThirdPersonCamera::GetNewCameraPosition(FVector orbitPosition) const
{
	const FVector direction = currentRotation.Vector();
	const FVector cameraPosition = orbitPosition + direction * target->CameraDistance;
	return cameraPosition;
}

FVector AThirdPersonCamera::GetCameraOffset() const
{
	return GetActorRightVector() * target->CameraOffset.X + FVector::UpVector * target->CameraOffset.Y;
}

FVector AThirdPersonCamera::GetCollidedCameraPosition(FVector orbitPosition, FVector cameraPosition)
{
	FHitResult hitResult;
	targetCollisionOffset = FVector::ZeroVector;
	FCollisionQueryParams params;
	params.AddIgnoredActor(target->GetOwner());
	if (GetWorld()->LineTraceSingleByChannel(hitResult, orbitPosition + FVector(0, 0, target->CameraOffset.Y),
	                                         cameraPosition, ECC_Camera, params))
	{
		targetCollisionOffset = hitResult.ImpactNormal * 10;
		cameraPosition = hitResult.ImpactPoint;
	}
	currentCollisionOffset = FMath::VInterpConstantTo(currentCollisionOffset, targetCollisionOffset,
	                                                  GetWorld()->GetDeltaSeconds(), 100);
	return cameraPosition + currentCollisionOffset;
}

void AThirdPersonCamera::ApplyRotation()
{
	currentRotation.Add(-mouseY * MouseSensitivity.Y, mouseX * MouseSensitivity.X, 0);
	currentRotation.Pitch = FMath::Clamp(currentRotation.Pitch, target->AngleBoundaries.X, target->AngleBoundaries.Y);
}

void AThirdPersonCamera::SetTarget(AActor* newTarget)
{
	if (const auto cameraParameters = Cast<UThirdPersonCameraParameters>(
		newTarget->GetComponentByClass(UThirdPersonCameraParameters::StaticClass())))
	{
		target = cameraParameters;
	}
}
