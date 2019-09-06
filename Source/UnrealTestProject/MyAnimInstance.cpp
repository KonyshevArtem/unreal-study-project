// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "Engine/Engine.h"

float UMyAnimInstance::GetHorizontalVelocityMagnitude() const
{
	FVector velocity = FVector(Velocity);
	velocity.Z = 0;
	return velocity.Size();
}

void UMyAnimInstance::EndInteract()
{
	endInteractEvent.Broadcast();
}

void UMyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	character = Cast<ACharacter>(GetOwningActor());
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!character) return;
	
	SetVelocity();
	SetLegState();
	SetIsFalling();
}

void UMyAnimInstance::SetVelocity()
{	
	const FVector inputVector = character->GetMovementComponent()->GetLastInputVector();
	InputMagnitude = inputVector.Size();
	Velocity = inputVector;
}

void UMyAnimInstance::SetLegState()
{
	const float legState = GetCurveValue(FName("LegState"));
	RoundedLegState = FMath::RoundHalfFromZero(legState);
}

void UMyAnimInstance::SetIsFalling()
{	
	IsFalling = character->GetMovementComponent()->IsFalling();
	RootMotionMode = IsFalling ? ERootMotionMode::IgnoreRootMotion : ERootMotionMode::RootMotionFromEverything;
}
