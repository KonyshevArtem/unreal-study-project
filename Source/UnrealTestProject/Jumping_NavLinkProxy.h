// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "MyAnimInstance.h"
#include "Jumping_NavLinkProxy.generated.h"

USTRUCT()
struct FSegment
{
	GENERATED_BODY()

public:
	FSegment() {}

	FSegment(FVector start, FVector end) : FSegment()
	{
		Start = start;
		End = end;
	};

	UPROPERTY()
		FVector Start;
	UPROPERTY()
		FVector End;
};

/**
 *
 */
UCLASS()
class UNREALTESTPROJECT_API AJumpingNavLinkProxy : public ANavLinkProxy
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void BeginJump(AActor* actor, const FVector& destination);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY()
		TMap<UMyAnimInstance*, FSegment> currentJumps;

	void RemoveJumpingActor(UMyAnimInstance* animInstance);
};
