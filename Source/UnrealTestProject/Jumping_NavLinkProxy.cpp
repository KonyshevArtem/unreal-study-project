// Fill out your copyright notice in the Description page of Project Settings.


#include "Jumping_NavLinkProxy.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

void AJumpingNavLinkProxy::BeginPlay()
{
	Super::BeginPlay();
	currentJumps = TMap<UMyAnimInstance*, FSegment>();
}

void AJumpingNavLinkProxy::BeginJump(AActor* actor, const FVector& destination)
{	
	ACharacter* character = Cast<ACharacter>(actor);
	UMyAnimInstance* animInstance = Cast<UMyAnimInstance>(character->GetMesh()->GetAnimInstance());
	animInstance->IsFalling = true;

	const FVector jumpStart = actor->GetActorLocation();
	const float zOffset = character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const FVector jumpEnd = destination + FVector(0, 0, zOffset);

	currentJumps.Add(animInstance, FSegment(jumpStart, jumpEnd));
}

void AJumpingNavLinkProxy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	for (const auto tuple : currentJumps)
	{
		UMyAnimInstance* animInstance = tuple.Key;
		FVector jumpStart = tuple.Value.Start;
		FVector jumpEnd = tuple.Value.End;
		
		const float progress = animInstance->GetCurveValue(FName("Progress"));
		const FVector location = FMath::Lerp(jumpStart, jumpEnd, progress);
		animInstance->GetOwningActor()->SetActorLocation(location);
		if (progress >= 0.9)
		{
			RemoveJumpingActor(animInstance);
		}
	}
}

void AJumpingNavLinkProxy::RemoveJumpingActor(UMyAnimInstance* animInstance)
{
	currentJumps.Remove(animInstance);
}
