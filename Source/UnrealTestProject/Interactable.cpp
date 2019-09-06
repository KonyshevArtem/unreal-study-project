// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"
#include "Engine/Engine.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime\Engine\Classes\Components\SkeletalMeshComponent.h"

void IInteractable::Tick(float DeltaTime)
{
	const FMoveToInteract moveToInteract = GetMoveToInteract();
	if (moveToInteract.character && moveToInteract.interactPoint)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(moveToInteract.character->GetController(),
			moveToInteract.interactPoint->GetComponentLocation());
		if (moveToInteract.HasReachedInteractPoint())
		{
			BeginInteract(moveToInteract.character);
			SetMoveToInteract(FMoveToInteract(nullptr, nullptr));
		}
	}
}

// Add default functionality here for any IInteractable functions that are not pure virtual.
void IInteractable::Interact(ACharacter* character)
{
	UInteractablePoint* closestInteractPoint = GetClosestInteractPoint(character, GetInteractPoints());
	if (closestInteractPoint)
	{
		SetMoveToInteract(FMoveToInteract(character, closestInteractPoint));
	}
}

void IInteractable::StopMoveToInteract(ACharacter* character)
{
	const FMoveToInteract moveToInteract = GetMoveToInteract();
	if (moveToInteract.character == character)
	{
		SetMoveToInteract(FMoveToInteract(nullptr, nullptr));
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
