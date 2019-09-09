// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameFramework/Character.h"
#include "InteractablePoint.h"
#include "MyAnimInstance.h"
#include "Interactable.generated.h"

class ActiveInteraction;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class UNREALTESTPROJECT_API IInteractable
{
	GENERATED_BODY()
protected:
	virtual TArray<UInteractablePoint*> GetInteractPoints() = 0;
	virtual void SetActiveInteraction(ActiveInteraction* activeInteraction) = 0;
	virtual ActiveInteraction* GetActiveInteraction() = 0;
	virtual void BeginInteract(ActiveInteraction* activeInteraction) = 0;
	virtual void InteractionTick(ActiveInteraction* activeInteraction) = 0;
	virtual float GetWarpSpeed() = 0;
	
	virtual void Tick(float DeltaTime);

public:
	ActiveInteraction* Interact(ACharacter* character);
	virtual void EndInteract(ActiveInteraction* activeInteraction);
	void StopMoveToInteract(ACharacter* character);

private:
	UInteractablePoint* GetClosestInteractPoint(ACharacter* character, TArray<UInteractablePoint*> interactPoints);
	float GetDistanceToInteractPoint(ACharacter* character, UInteractablePoint* interactPoint);
};