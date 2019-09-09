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
	ActiveInteraction(ACharacter* character, UInteractablePoint* interactPoint, TFunction<void(ActiveInteraction*)> onReachInteractPoint);
	virtual ~ActiveInteraction() = default;

private:
	TFunction<void(ActiveInteraction*)> onReachInteractPoint;
	bool isWarping = false;
	float totalWarpTime, currentWarpTime;
	
	bool HasReachedInteractPoint() const;
	void Warp(float deltaTime, float warpSpeed);
	
public:
	ACharacter* Character;
	UInteractablePoint* InteractPoint;
	UMyAnimInstance* CharacterAnimInstance;
	bool IsInteracting = false;

	void MoveToInteract(float deltaTime, float warpSpeed);
};
