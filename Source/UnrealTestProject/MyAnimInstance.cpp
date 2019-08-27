// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "Engine/Engine.h"

float UMyAnimInstance::GetHorizontalVelocityMagnitude() const
{
	FVector velocity = FVector(Velocity);
	velocity.Z = 0;
	return velocity.Size();
}

void UMyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	character = Cast<ACharacter>(GetOwningActor());
	axisMovement = Cast<UAxisMovement>(character->GetComponentByClass(UAxisMovement::StaticClass()));
	if (!axisMovement)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "[Error] No AxisMovement component on owning actor");
		UE_LOG(LogTemp, Error, TEXT("No AxisMovement component on owning actor"))
	}
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!axisMovement) return;

	SetVelocity();
	SetLegState();
	SetIsFalling();
}

void UMyAnimInstance::SetVelocity()
{
	FVector2D inputVector = FVector2D(axisMovement->Horizontal, axisMovement->Vertical);
	inputVector.Normalize();
	InputMagnitude = inputVector.Size();
	Velocity = character->GetMovementComponent()->Velocity.GetSafeNormal();
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
