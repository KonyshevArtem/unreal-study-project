// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCar.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "ThirdPersonCamera.h"
#include "Components/SkeletalMeshComponent.h"
#include "MyAnimInstance.h"
#include "GameFramework/PlayerController.h"
#include "ActiveInteraction.h"

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
	GetComponents<UInteractablePoint>(interactPoints);
	carAnimInstance = GetMesh()->GetAnimInstance();
}

TArray<UInteractablePoint*> AMyCar::GetInteractPoints()
{
	return interactPoints;
}

void AMyCar::SetActiveInteraction(ActiveInteraction* activeInteraction)
{
	currentActiveInteraction = TSharedPtr<ActiveInteraction>(activeInteraction);
}

ActiveInteraction* AMyCar::GetActiveInteraction()
{
	return currentActiveInteraction.Get();
}

void AMyCar::InteractionTick(ActiveInteraction* activeInteraction)
{
	if (!activeInteraction->CharacterAnimInstance) return;

	const float weight = activeInteraction->CharacterAnimInstance->GetCurveValue("DriverSocketWeight");
	if (weight > 0)
	{
		const FVector driverLocation = FMath::Lerp(activeInteraction->InteractPoint->GetComponentLocation(),
			GetMesh()->GetSocketLocation("DriverSocket"), weight);
		const FRotator driverRotation = FMath::Lerp(activeInteraction->InteractPoint->GetComponentRotation(),
			GetMesh()->GetSocketRotation("DriverSocket"), weight);
		activeInteraction->Character->SetActorLocation(driverLocation);
		activeInteraction->Character->SetActorRotation(driverRotation);
	}
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
	if (!driver) return;

	if (IsPlayerControlled())
	{
		AThirdPersonCamera* camera = Cast<AThirdPersonCamera>(GetWorld()->GetFirstPlayerController()->GetViewTarget());
		camera->SetTarget(driver);
	}

	GetWorld()->GetFirstPlayerController()->UnPossess();
	GetWorld()->GetFirstPlayerController()->Possess(driver);

	driver->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	driver->SetActorLocation(interactPoints[0]->GetComponentLocation());
	driver->SetActorEnableCollision(true);
	if (driverAnimInstance)
	{
		driverAnimInstance->IsDriving = false;
	}
}

void AMyCar::BeginInteract(ActiveInteraction* activeInteraction)
{
	driver = activeInteraction->Character;

	if (driver->IsPlayerControlled())
	{
		AThirdPersonCamera* camera = Cast<AThirdPersonCamera>(GetWorld()->GetFirstPlayerController()->GetViewTarget());
		camera->SetTarget(this);
	}

	GetWorld()->GetFirstPlayerController()->UnPossess();

	if (carAnimInstance && activeInteraction->InteractPoint->InteractableItemMontage)
	{
		carAnimInstance->Montage_Play(activeInteraction->InteractPoint->InteractableItemMontage);
	}

	driver->SetActorEnableCollision(false);
	if (UAnimInstance * animInstance = driver->GetMesh()->GetAnimInstance())
	{
		driverAnimInstance = Cast<UMyAnimInstance>(animInstance);
		if (activeInteraction->InteractPoint->CharacterInteractMontage)
		{
			driver->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, FName("DriverSocket"));
			Cast<UCharacterMovementComponent>(driver->GetMovementComponent())->SetMovementMode(MOVE_Flying);
			driver->PlayAnimMontage(activeInteraction->InteractPoint->CharacterInteractMontage);
		}
		else
		{
			driver->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("DriverSocket"));
			EndInteract(activeInteraction);
		}
	}
	else
	{
		driver->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("DriverSocket"));
		EndInteract(activeInteraction);
	}
}

void AMyCar::EndInteract(ActiveInteraction* activeInteraction)
{
	IInteractable::EndInteract(activeInteraction);

	GetWorld()->GetFirstPlayerController()->Possess(this);
	if (activeInteraction->CharacterAnimInstance)
	{
		Cast<UCharacterMovementComponent>(activeInteraction->Character->GetMovementComponent())->SetMovementMode(MOVE_Walking);
		activeInteraction->CharacterAnimInstance->IsDriving = true;
	}
}

void AMyCar::Tick(float DeltaTime)
{
	AWheeledVehicle::Tick(DeltaTime);
	IInteractable::Tick(DeltaTime);
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
