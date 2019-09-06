// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameFramework/Character.h"
#include "InteractablePoint.h"
#include "Interactable.generated.h"

USTRUCT(BlueprintType)
struct UNREALTESTPROJECT_API FMoveToInteract
{
	GENERATED_BODY()

public:
	FMoveToInteract() = default;
	FMoveToInteract(ACharacter* character, UInteractablePoint* interactPoint)
	{
		this->character = character;
		this->interactPoint = interactPoint;
	}
	virtual ~FMoveToInteract() = default;

	UPROPERTY()
		ACharacter* character;
	UPROPERTY()
		UInteractablePoint* interactPoint;

	bool HasReachedInteractPoint() const
	{
		const float distanceToPoint = FVector::Distance(character->GetActorLocation(), interactPoint->GetComponentLocation());
		return distanceToPoint < 100;
	}
};

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
	virtual void SetMoveToInteract(FMoveToInteract moveToInteract) = 0;
	virtual FMoveToInteract GetMoveToInteract() = 0;
	virtual void BeginInteract(ACharacter* character) = 0;
	
	virtual void Tick(float DeltaTime);

public:
	void Interact(ACharacter* character);
	virtual void EndInteract(ACharacter* character) = 0;
	void StopMoveToInteract(ACharacter* character);

private:
	UInteractablePoint* GetClosestInteractPoint(ACharacter* character, TArray<UInteractablePoint*> interactPoints);
	float GetDistanceToInteractPoint(ACharacter* character, UInteractablePoint* interactPoint);
};
