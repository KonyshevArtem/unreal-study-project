// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "WheeledVehicleMovementComponent.h"
#include "GameFramework/Character.h"
#include "MyAnimInstance.h"
#include "Interactable.h"
#include "InteractablePoint.h"
#include "MyCar.generated.h"

/**
 *
 */
UCLASS()
class UNREALTESTPROJECT_API AMyCar : public AWheeledVehicle, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BeginInteract(ACharacter* character) override;
	virtual void EndInteract(ACharacter* character) override;
	virtual TArray<UInteractablePoint*> GetInteractPoints() override;
	virtual void SetMoveToInteract(FMoveToInteract moveToInteract) override;
	virtual FMoveToInteract GetMoveToInteract() override;

private:
	UPROPERTY()
		ACharacter* driver;
	UPROPERTY()
		UWheeledVehicleMovementComponent* movementComponent;
	UPROPERTY()
		UMyAnimInstance* driverAnimInstance;
	UPROPERTY()
		FMoveToInteract currentMoveToInteract;

	void SetSteering(float axisValue);
	void SetThrottle(float axisValue);
	void EnableHandbrake();
	void DisableHandbrake();
	void GetOutOfCar();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Car properties")
		TArray<UInteractablePoint*> InteractPoints;

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
