// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AxisMovement.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALTESTPROJECT_API UAxisMovement : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAxisMovement();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	float Vertical, Horizontal;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
		ACharacter* ownerCharacter;
	UPROPERTY()
		UCharacterMovementComponent* movementComponent;
	UPROPERTY()
		ACameraActor* mainCamera;

	void RotateActorToVelocity(float DeltaTime);
	void SetVertical(float axisValue) { Vertical = axisValue; }
	void SetHorizontal(float axisValue) { Horizontal = axisValue; }
	void Jump() { ownerCharacter->Jump(); }
};
