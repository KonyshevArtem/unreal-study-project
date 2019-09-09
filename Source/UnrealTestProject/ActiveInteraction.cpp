// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveInteraction.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

ActiveInteraction::ActiveInteraction(ACharacter* character, UInteractablePoint* interactPoint,
	TFunction<void(ActiveInteraction*)> onReachInteractPoint)
{
	this->Character = character;
	this->InteractPoint = interactPoint;
	this->onReachInteractPoint = onReachInteractPoint;
	this->IsInteracting = false;
	if (character)
	{
		if (UAnimInstance * animInstance = character->GetMesh()->GetAnimInstance())
		{
			this->CharacterAnimInstance = Cast<UMyAnimInstance>(animInstance);
		}
	}
}

bool ActiveInteraction::HasReachedInteractPoint() const
{
	const float distanceToPoint = FVector::Distance(Character->GetActorLocation(), InteractPoint->GetComponentLocation());
	return distanceToPoint < 100;
}

void ActiveInteraction::Warp(float deltaTime, float warpSpeed)
{
	const FVector newPosition = FMath::VInterpConstantTo(Character->GetActorLocation(),
	                                                     InteractPoint->GetComponentLocation(), deltaTime, warpSpeed);
	const FRotator newRotation = FMath::RInterpConstantTo(Character->GetActorRotation(),
	                                                      InteractPoint->GetComponentRotation(), deltaTime, warpSpeed);

	Character->SetActorLocation(newPosition);
	Character->SetActorRotation(newRotation);
	
	currentWarpTime += deltaTime;
	if (currentWarpTime >= totalWarpTime)
	{
		isWarping = false;
		IsInteracting = true;
		if (onReachInteractPoint)
		{
			onReachInteractPoint(this);
		}
	}
}

void ActiveInteraction::MoveToInteract(float deltaTime, float warpSpeed)
{
	if (!Character || !InteractPoint) return;

	if (!isWarping)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(Character->GetController(), InteractPoint->GetComponentLocation());
		if (HasReachedInteractPoint())
		{
			const float positionDistance = FVector::Distance(Character->GetActorLocation(), InteractPoint->GetComponentLocation());
			const float rotationDistance = Character->GetActorRotation().GetManhattanDistance(InteractPoint->GetComponentRotation());
			totalWarpTime = FMath::Max(positionDistance, rotationDistance) / warpSpeed;
			currentWarpTime = 0;
			isWarping = true;
		}
	}
	else
	{
		Warp(deltaTime, warpSpeed);
	}
}
