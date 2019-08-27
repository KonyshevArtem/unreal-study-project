// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
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

	void SetMouseX(float value) { mouseX = value; }
	void SetMouseY(float value) { mouseY = value; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera parameters")
		float CameraDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera parameters")
		FVector2D CameraOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera parameters")
		FVector2D MouseSensitivity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera parameters")
		FVector2D AngleBoundaries;
};
