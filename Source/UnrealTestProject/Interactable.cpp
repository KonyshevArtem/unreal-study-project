// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"
#include "Engine/Engine.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "ActiveInteraction.h"

void IInteractable::Tick(float DeltaTime)
{
	ActiveInteraction* activeInteraction = GetActiveInteraction();
	if (!activeInteraction) return;
	
	if (!activeInteraction->IsInteracting)
	{
		activeInteraction->MoveToInteract();
	}
	else
	{
		InteractionTick(activeInteraction);
	}
}

// Add default functionality here for any IInteractable functions that are not pure virtual.
void IInteractable::Interact(ACharacter* character)
{
	UInteractablePoint* closestInteractPoint = GetClosestInteractPoint(character, GetInteractPoints());
	if (closestInteractPoint)
	{
		SetActiveInteraction(new ActiveInteraction(character, closestInteractPoint, [this](ACharacter* character)
			{
				BeginInteract(character);
			}));
	}
}

void IInteractable::EndInteract(ACharacter* character)
{
	SetActiveInteraction(nullptr);
}

void IInteractable::StopMoveToInteract(ACharacter* character)
{
	ActiveInteraction* activeInteraction = GetActiveInteraction();
	if (activeInteraction && activeInteraction->Character == character)
	{
		SetActiveInteraction(nullptr);
	}
}

UInteractablePoint* IInteractable::GetClosestInteractPoint(ACharacter* character, TArray<UInteractablePoint*> interactPoints)
{
	if (interactPoints.Num() == 0) return nullptr;
	if (interactPoints.Num() == 1) return interactPoints[0];

	UInteractablePoint* closestInteractPoint = interactPoints[0];
	float closestInteractPointDistance = GetDistanceToInteractPoint(character, closestInteractPoint);
	for (int i = 1; i < interactPoints.Num(); ++i)
	{
		const float current = GetDistanceToInteractPoint(character, interactPoints[i]);
		if (current < closestInteractPointDistance)
		{
			closestInteractPointDistance = current;
			closestInteractPoint = interactPoints[i];
		}
	}
	return closestInteractPoint;
}

float IInteractable::GetDistanceToInteractPoint(ACharacter* character, UInteractablePoint* interactPoint)
{
	UNavigationPath* path = UNavigationSystemV1::FindPathToLocationSynchronously(
		character, character->GetActorLocation(), interactPoint->GetComponentLocation());
	return path->GetPathLength();
}
