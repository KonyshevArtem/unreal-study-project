// Fill out your copyright notice in the Description page of Project Settings.


#include "MouseClickMovement.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "AIController.h"

// Sets default values for this component's properties
UMouseClickMovement::UMouseClickMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UMouseClickMovement::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* ownerCharacter = Cast<ACharacter>(GetOwner());
	AIController = Cast<AAIController>(ownerCharacter->GetController());
	if (!AIController)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString("[Error] No AIController on owner character"));
		UE_LOG(LogTemp, Error, TEXT("No AIController on owner character"));
	}
	
	playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (playerController)
	{
		playerController->InputComponent->BindAction("LMB", IE_Pressed, this, &UMouseClickMovement::OnMouseClick);
		playerController->bShowMouseCursor = true;
		playerController->bEnableClickEvents = true;
		playerController->bEnableMouseOverEvents = true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString("[Error] No player controller found"));
		UE_LOG(LogTemp, Error, TEXT("No player controller found"));
	}
}


// Called every frame
void UMouseClickMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMouseClickMovement::OnMouseClick()
{
	if (!playerController || !AIController)
	{
		return;
	}
	
	FVector position, end;
	GetTraceStartEnd(position, end);

	FHitResult hitResult;
	const bool isHit = GetWorld()->LineTraceSingleByChannel(hitResult, position, end, ECC_GameTraceChannel1);
	if (isHit)
	{
		AIController->MoveToLocation(hitResult.ImpactPoint);
	}
}

void UMouseClickMovement::GetTraceStartEnd(FVector& position, FVector& end) const
{
	float mouseX, mouseY;
	playerController->GetMousePosition(mouseX, mouseY);
	FVector direction;
	UGameplayStatics::DeprojectScreenToWorld(playerController, FVector2D(mouseX, mouseY), position, direction);
	end = position + direction * 1e6;
}

