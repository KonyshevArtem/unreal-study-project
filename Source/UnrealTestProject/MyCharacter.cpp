// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Engine.h"
#include "MyUtils.h"
#include "AxisMovement.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMyCharacter::OpenDoor()
{
	if (!OpenDoorAnimMontage)
	{
		MyUtils::LogError("No open door anim montage set");
		return;
	}
	if (GetCurrentMontage() != OpenDoorAnimMontage)
	{
		PlayAnimMontage(OpenDoorAnimMontage);
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	TSet<UActorComponent*> components = GetComponents();
	for (UActorComponent* component : components)
	{
		if (IRequireInput* componentWithInput = Cast<IRequireInput>(component))
		{
			IRequireInput::Execute_InitializeInput(component, PlayerInputComponent);
		}
	}
}



