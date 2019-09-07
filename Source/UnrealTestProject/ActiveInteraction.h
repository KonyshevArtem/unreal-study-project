// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyAnimInstance.h"
#include "InteractablePoint.h"

/**
 *
 */
class UNREALTESTPROJECT_API ActiveInteraction
{
public:
	ActiveInteraction(ACharacter* character, UInteractablePoint* interactPoint, TFunction<void(ACharacter*)> onReachInteractPoint);
	virtual ~ActiveInteraction() = default;

private:
	TFunction<void(ACharacter*)> onReachInteractPoint;

	bool HasReachedInteractPoint() const;

public:
	ACharacter* Character;
	UInteractablePoint* InteractPoint;
	UMyAnimInstance* CharacterAnimInstance;
	bool IsInteracting;

	void MoveToInteract();
};
