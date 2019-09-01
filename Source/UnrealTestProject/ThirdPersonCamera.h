// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "ThirdPersonCameraParameters.h"
#include "ThirdPersonCamera.generated.h"

/**
 *
 */
UCLASS()
class UNREALTESTPROJECT_API AThirdPersonCamera : public ACameraActor
{
	GENERATED_BODY()

public:
	AThirdPersonCamera();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	float mouseX, mouseY;

	UPROPERTY()
		FRotator currentRotation;
	UPROPERTY()
		FVector currentCollisionOffset;
	UPROPERTY()
		FVector targetCollisionOffset;
	UPROPERTY()
		UThirdPersonCameraParameters* target;

	void SetMouseX(float value) { mouseX = value; }
	void SetMouseY(float value) { mouseY = value; }

	FVector GetNewCameraPosition(FVector orbitPosition) const;
	FVector GetCameraOffset() const;
	FVector GetCollidedCameraPosition(FVector orbitPosition, FVector cameraPosition);
	void ApplyRotation();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera parameters")
		FVector2D MouseSensitivity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera parameters")
		float RotationSpeed = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera parameters")
		float PositionSpeed = 50;

	UFUNCTION(BlueprintCallable)
		void SetTarget(AActor* newTarget);
};
