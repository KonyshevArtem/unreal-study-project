// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractComponent.h"
#include "Engine/Engine.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "MyUtils.h"
#include "GameFramework/Controller.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	UActorComponent::BeginPlay();

	ownerCharacter = Cast<ACharacter>(GetOwner());
	if (!ownerCharacter)
	{
		MyUtils::LogError("Driver component requires ACharacter as owner");
	}

	if (UMyAnimInstance* animInstance = Cast<UMyAnimInstance>(ownerCharacter->GetMesh()->GetAnimInstance()))
	{
		animInstance->OnEndInteract().AddUFunction(this, "EndInteract");
	}
	ownerCharacter->OnActorBeginOverlap.AddDynamic(this, &UInteractComponent::ActorBeginOverlap);
	ownerCharacter->OnActorEndOverlap.AddDynamic(this, &UInteractComponent::ActorEndOverlap);
}

void UInteractComponent::EndInteract() const
{
	if (targetInteractable.GetObject())
	{
		targetInteractable->EndInteract(ownerCharacter);
	}
}


// Called every frame
void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ownerCharacter && ownerCharacter->GetMovementComponent()->IsFalling())
	{
		StopInteracting();
	}
}

void UInteractComponent::InitializeInput_Implementation(UInputComponent* inputComponent)
{
	inputComponent->BindAction("Interact", IE_Released, this, &UInteractComponent::Interact);
	inputComponent->BindAxis("Horizontal", this, &UInteractComponent::ReceiveAxisInput);
	inputComponent->BindAxis("Vertical", this, &UInteractComponent::ReceiveAxisInput);
}

void UInteractComponent::ActorBeginOverlap(AActor* thisActor, AActor* otherActor)
{
	if (otherActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		triggeredInteractable.SetInterface(Cast<IInteractable>(otherActor));
		triggeredInteractable.SetObject(otherActor);
	}
}

void UInteractComponent::ActorEndOverlap(AActor* thisActor, AActor* otherActor)
{
	if (otherActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		triggeredInteractable.SetObject(nullptr);
	}
}

void UInteractComponent::StopInteracting()
{
	if (targetInteractable.GetObject() && ownerCharacter)
	{
		targetInteractable->StopMoveToInteract(ownerCharacter);
		targetInteractable.SetObject(nullptr);
		ownerCharacter->GetMovementComponent()->StopActiveMovement();
	}
}

void UInteractComponent::ReceiveAxisInput(float axisValues)
{
	if (FMath::Abs(axisValues) > 0.1f)
	{
		StopInteracting();
	}
}

void UInteractComponent::Interact()
{
	if (!triggeredInteractable.GetObject() || !ownerCharacter) return;

	targetInteractable.SetInterface(triggeredInteractable.GetInterface());
	targetInteractable.SetObject(triggeredInteractable.GetObject());
	triggeredInteractable->Interact(ownerCharacter);
}