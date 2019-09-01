// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThirdPersonCameraParameters.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALTESTPROJECT_API UThirdPersonCameraParameters : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UThirdPersonCameraParameters() = default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera parameters")
		float CameraDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera parameters")
		FVector2D CameraOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera parameters")
		FVector2D AngleBoundaries;
};
