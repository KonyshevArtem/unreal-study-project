// Fill out your copyright notice in the Description page of Project Settings.


#include "DrivingComponent.h"
#include "Engine/Engine.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Runtime/AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "MyUtils.h"
#include "GameFramework/Controller.h"

// Sets default values for this component's properties
UDrivingComponent::UDrivingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDrivingComponent::BeginPlay()
{
	UActorComponent::BeginPlay();

	ownerCharacter = Cast<ACharacter>(GetOwner());
	if (!ownerCharacter)
	{
		MyUtils::LogError("Driver component requires ACharacter as owner");
	}
	ownerController = ownerCharacter->GetController();
	
	ownerCharacter->OnActorBeginOverlap.AddDynamic(this, &UDrivingComponent::ActorBeginOverlap);
	ownerCharacter->OnActorEndOverlap.AddDynamic(this, &UDrivingComponent::ActorEndOverlap);
}


// Called every frame
void UDrivingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (isEnteringCar)
	{
		if (ownerCharacter->GetMovementComponent()->IsFalling())
		{
			isEnteringCar = false;
		}
		else if (HasReachedCarEnter())
		{
			targetCar->GetInCar(ownerCharacter);
		}
	}
}

bool UDrivingComponent::HasReachedCarEnter() const
{
	if (!isEnteringCar) return false;
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(ownerController, carEnterPoint->GetComponentLocation());

	const float distanceToCar = FVector::Distance(GetOwner()->GetActorLocation(), carEnterPoint->GetComponentLocation());
	return distanceToCar < 100;
}

void UDrivingComponent::InitializeInput_Implementation(UInputComponent* inputComponent)
{
	inputComponent->BindAction("Interact", IE_Released, this, &UDrivingComponent::GoToCar);
	inputComponent->BindAxis("Horizontal", this, &UDrivingComponent::StopGoingToCar);
	inputComponent->BindAxis("Vertical", this, &UDrivingComponent::StopGoingToCar);
}

void UDrivingComponent::ActorBeginOverlap(AActor* thisActor, AActor* otherActor)
{
	if (AMyCar* car = Cast<AMyCar>(otherActor))
	{
		triggeredCar = car;
	}
}

void UDrivingComponent::ActorEndOverlap(AActor* thisActor, AActor* otherActor)
{
	if (AMyCar * car = Cast<AMyCar>(otherActor))
	{
		if (car == triggeredCar)
		{
			triggeredCar = nullptr;
		}
	}
}

void UDrivingComponent::StopGoingToCar(float axisValues)
{
	if (FMath::Abs(axisValues) > 0.1f)
	{
		isEnteringCar = false;
	}
}

void UDrivingComponent::GoToCar()
{
	if (!triggeredCar || isEnteringCar) return;

	targetCar = triggeredCar;
	carEnterPoint = GetClosestEnterPoint(targetCar);
	isEnteringCar = carEnterPoint != nullptr;
}

USceneComponent* UDrivingComponent::GetClosestEnterPoint(AMyCar* car)
{
	if (!car->DriverEntryPoint || !car->PassengerEntryPoint)
	{
		MyUtils::LogError("Target car has no enter points");
		return nullptr;
	}
	
	const float driverDistance = GetDistanceToEnterPoint(car->DriverEntryPoint);
	const float passengerDistance = GetDistanceToEnterPoint(car->PassengerEntryPoint);
	return driverDistance < passengerDistance ? car->DriverEntryPoint : car->PassengerEntryPoint;
}

float UDrivingComponent::GetDistanceToEnterPoint(USceneComponent* enterPoint)
{
	UNavigationPath* path = UNavigationSystemV1::FindPathToLocationSynchronously(
		this, GetOwner()->GetActorLocation(), enterPoint->GetComponentLocation());
	return path->GetPathLength();
}

