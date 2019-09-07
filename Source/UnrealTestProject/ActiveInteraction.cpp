// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveInteraction.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

ActiveInteraction::ActiveInteraction(ACharacter* character, UInteractablePoint* interactPoint,
	TFunction<void(ACharacter*)> onReachInteractPoint)
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

void ActiveInteraction::MoveToInteract()
{
	if (!Character || !InteractPoint) return;
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(Character->GetController(), InteractPoint->GetComponentLocation());
	if (HasReachedInteractPoint())
	{
		IsInteracting = true;
		if (onReachInteractPoint)
		{
			onReachInteractPoint(Character);
		}
	}
}
