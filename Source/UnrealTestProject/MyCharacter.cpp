// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Engine.h"

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
		UE_LOG(LogTemp, Error, TEXT("No open door anim montage set"))
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "[Error] No open door anim montage set");
		return;
	}
	PlayAnimMontage(OpenDoorAnimMontage);
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
}



