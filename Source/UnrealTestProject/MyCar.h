// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "WheeledVehicleMovementComponent.h"
#include "GameFramework/Character.h"
#include "MyCar.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTESTPROJECT_API AMyCar : public AWheeledVehicle
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AMyCar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
		UWheeledVehicleMovementComponent* movementComponent;
	
	void SetSteering(float axisValue);
	void SetThrottle(float axisValue);
	void EnableHandbrake();
	void DisableHandbrake();
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Car properties")
		ACharacter* Driver;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Car properties")
		USceneComponent* DriverEntryPoint;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Car properties")
		USceneComponent* PassengerEntryPoint;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
