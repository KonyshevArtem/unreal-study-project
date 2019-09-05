// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCar.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "ThirdPersonCamera.h"
#include "Components/SkeletalMeshComponent.h"
#include "MyAnimInstance.h"
#include "GameFramework/PlayerController.h"

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

void AMyCar::GetOutOfCar()
{
	if (!Driver) return;

	if (IsPlayerControlled())
	{
		AThirdPersonCamera* camera = Cast<AThirdPersonCamera>(GetWorld()->GetFirstPlayerController()->GetViewTarget());
		camera->SetTarget(Driver);
	}

	GetWorld()->GetFirstPlayerController()->UnPossess();
	GetWorld()->GetFirstPlayerController()->Possess(Driver);

	Driver->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Driver->SetActorLocation(DriverEntryPoint->GetComponentLocation());
	Driver->SetActorEnableCollision(true);
	if (UAnimInstance * animInstance = Driver->GetMesh()->GetAnimInstance())
	{
		UMyAnimInstance* myAnimInstance = Cast<UMyAnimInstance>(animInstance);
		myAnimInstance->IsDriving = false;
	}
}

void AMyCar::GetInCar(ACharacter* driver)
{
	Driver = driver;

	/*driver->GetController()->UnPossess();
	GetController()->Possess(this);*/

	if (driver->IsPlayerControlled())
	{
		AThirdPersonCamera* camera = Cast<AThirdPersonCamera>(GetWorld()->GetFirstPlayerController()->GetViewTarget());
		camera->SetTarget(this);
	}
	
	GetWorld()->GetFirstPlayerController()->UnPossess();
	GetWorld()->GetFirstPlayerController()->Possess(this);

	const FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
	                                      EAttachmentRule::KeepWorld, true);
	driver->AttachToActor(this, rules, FName("DriverSocket"));
	driver->SetActorEnableCollision(false);
	if (UAnimInstance* animInstance = driver->GetMesh()->GetAnimInstance())
	{
		UMyAnimInstance* myAnimInstance = Cast<UMyAnimInstance>(animInstance);
		myAnimInstance->IsDriving = true;
	}
	
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
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &AMyCar::GetOutOfCar);
}
