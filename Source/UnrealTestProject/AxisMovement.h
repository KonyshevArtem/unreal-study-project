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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Axis movement parameters")
		float JumpSpeed = 500;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void InitializeInput(UInputComponent* InputComponent);

private:
	float vertical, horizontal;
	
	UPROPERTY()
		ACharacter* ownerCharacter;
	UPROPERTY()
		UCharacterMovementComponent* movementComponent;
	UPROPERTY()
		ACameraActor* mainCamera;
	UPROPERTY()
		UAnimInstance* animInstance;

	void SetMainCamera();
	FVector GetMoveInput() const;
	void AddMoveInput();
	void RotateActorToVelocity(float DeltaTime) const;
	void SetVertical(float axisValue) { vertical = axisValue; }
	void SetHorizontal(float axisValue) { horizontal = axisValue; }
	void Jump();
};
