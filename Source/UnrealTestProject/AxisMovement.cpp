// Fill out your copyright notice in the Description page of Project Settings.


#include "AxisMovement.h"
#include "GameFramework/Character.h"
#include "Engine/Engine.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "MyUtils.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "AIController.h"
#include "UnrealTestProject.h"

// Sets default values for this component's properties
UAxisMovement::UAxisMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAxisMovement::BeginPlay()
{
	UActorComponent::BeginPlay();

	ownerCharacter = Cast<ACharacter>(GetOwner());
	ownerCharacter->MovementModeChangedDelegate.AddDynamic(this, &UAxisMovement::OnMovementModeChanged);
	USkeletalMeshComponent* mesh = ownerCharacter->GetMesh();
	if (mesh)
	{
		animInstance = mesh->GetAnimInstance();
	}

	movementComponent = ownerCharacter->GetCharacterMovement();
	if (!movementComponent)
	{
		MyUtils::LogError("No movement component on Axis Movement owner");
	}

	UInputComponent* inputComponent = ownerCharacter->InputComponent;
	if (!inputComponent)
	{
		MyUtils::LogError("No input component on owner of Axis Movement component");
		return;
	}
	InitializeInput(inputComponent);
}


// Called every frame
void UAxisMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SetMainCamera();
	AddMoveInput();
	RotateActorToVelocity(DeltaTime);
}

void UAxisMovement::InitializeInput_Implementation(UInputComponent* inputComponent)
{
	inputComponent->BindAxis("Vertical", this, &UAxisMovement::SetVertical);
	inputComponent->BindAxis("Horizontal", this, &UAxisMovement::SetHorizontal);
	inputComponent->BindAction("Jump", IE_Pressed, this, &UAxisMovement::Jump);
}


void UAxisMovement::SetMainCamera()
{
	if (!mainCamera)
	{
		mainCamera = Cast<ACameraActor>(ownerCharacter->Controller->GetViewTarget());
	}
}

FVector UAxisMovement::GetMoveInput() const
{
	if (!mainCamera) return FVector::ZeroVector;

	const FVector cameraForward = mainCamera->GetActorForwardVector();
	const FVector cameraRight = mainCamera->GetActorRightVector();
	FVector moveInput = cameraForward * vertical + cameraRight * horizontal;
	moveInput.Z = 0;
	moveInput.Normalize();
	return moveInput;
}

void UAxisMovement::AddMoveInput()
{
	const FVector moveInput = GetMoveInput();
	if (moveInput.Size() > 0.1f)
	{
		movementComponent->StopActiveMovement();
	}
	ownerCharacter->AddMovementInput(moveInput);
}

void UAxisMovement::RotateActorToVelocity(float DeltaTime)
{
	if (!mainCamera || !movementComponent) return;

	const FVector moveInput = movementComponent->GetLastInputVector();
	if (moveInput.Size() > 0.1 && !movementComponent->IsFalling())
	{
		const FRotator newRotation = FMath::Lerp(ownerCharacter->GetActorRotation(), moveInput.Rotation(), DeltaTime * 10);
		ownerCharacter->SetActorRotation(newRotation);
		lastGroundedMoveInput = moveInput;
	}
	else if (movementComponent->IsFalling())
	{
		FVector velocity = movementComponent->Velocity;
		velocity.Z = 0;
		if (velocity.Size() < 0.1)
		{
			velocity = lastGroundedMoveInput;
		}
		const FRotator newRotation = FMath::Lerp(ownerCharacter->GetActorRotation(), velocity.Rotation(), DeltaTime * 10);
		ownerCharacter->SetActorRotation(newRotation);
	}
}

void UAxisMovement::Jump()
{
	if (movementComponent->IsFalling()) return;

	const FVector moveInput = GetMoveInput();
	movementComponent->StopActiveMovement();
	ownerCharacter->Jump();
	movementComponent->Velocity = moveInput * JumpSpeed;
}

void UAxisMovement::OnMovementModeChanged(ACharacter* character, EMovementMode prevMovementMode, uint8 prevCustomMode)
{
	if (!animInstance) return;

	if (movementComponent->MovementMode == MOVE_Walking)
	{
		animInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
	}
	else if (movementComponent->MovementMode == MOVE_Falling)
	{
		animInstance->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
	}
}
