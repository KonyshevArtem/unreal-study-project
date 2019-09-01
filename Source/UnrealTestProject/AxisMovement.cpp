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
	Super::BeginPlay();

	ownerCharacter = Cast<ACharacter>(GetOwner());
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
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	RotateActorToVelocity(DeltaTime);
}

void UAxisMovement::InitializeInput(UInputComponent* inputComponent)
{
	inputComponent->BindAxis("Vertical", this, &UAxisMovement::SetVertical);
	inputComponent->BindAxis("Horizontal", this, &UAxisMovement::SetHorizontal);
	inputComponent->BindAction("Jump", IE_Pressed, this, &UAxisMovement::Jump);
}


FVector UAxisMovement::GetMoveInput() const
{
	const FVector cameraForward = mainCamera->GetActorForwardVector();
	const FVector cameraRight = mainCamera->GetActorRightVector();
	FVector moveInput = cameraForward * Vertical + cameraRight * Horizontal;
	moveInput.Z = 0;
	moveInput.Normalize();
	return moveInput;
}

void UAxisMovement::RotateActorToVelocity(float DeltaTime)
{
	if (!mainCamera)
	{
		mainCamera = Cast<ACameraActor>(ownerCharacter->Controller->GetViewTarget());
	}
	if (!mainCamera || !movementComponent) return;

	const FVector moveInput = GetMoveInput();
	if (moveInput.Size() > 0.1 && !movementComponent->IsFalling())
	{
		const FRotator newRotation = FMath::Lerp(ownerCharacter->GetActorRotation(), moveInput.Rotation(), DeltaTime * 10);
		ownerCharacter->SetActorRotation(newRotation);
	}
	else if (movementComponent->IsFalling())
	{
		FVector velocity = movementComponent->Velocity;
		velocity.Z = 0;
		const FRotator newRotation = FMath::Lerp(ownerCharacter->GetActorRotation(), velocity.Rotation(), DeltaTime * 10);
		ownerCharacter->SetActorRotation(newRotation);
	}
}

void UAxisMovement::Jump()
{
	const FVector moveInput = GetMoveInput();
	if (animInstance)
	{
		animInstance->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
	}
	ownerCharacter->Jump();
	movementComponent->Velocity = moveInput * JumpSpeed;
}

