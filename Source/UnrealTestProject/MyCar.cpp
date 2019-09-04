// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCar.h"
#include "Components/InputComponent.h"

// Sets default values
AMyCar::AMyCar()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

void AMyCar::BeginPlay()
{
	Super::BeginPlay();

	movementComponent = GetVehicleMovementComponent();
}

void AMyCar::SetSteering(float axisValue)
{
	movementComponent->SetSteeringInput(axisValue);
}

void AMyCar::SetThrottle(float axisValue)
{
	movementComponent->SetThrottleInput(axisValue);
}

void AMyCar::EnableHandbrake()
{
	movementComponent->SetHandbrakeInput(true);
}

void AMyCar::DisableHandbrake()
{
	movementComponent->SetHandbrakeInput(false);
}

void AMyCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyCar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Vertical", this, &AMyCar::SetThrottle);
	PlayerInputComponent->BindAxis("Horizontal", this, &AMyCar::SetSteering);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyCar::EnableHandbrake);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyCar::DisableHandbrake);
}
