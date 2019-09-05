// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AxisMovement.h"
#include "MyAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class UNREALTESTPROJECT_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation parameters")
		FVector Velocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation parameters")
		float InputMagnitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation parameters")
		float RoundedLegState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation parameters")
		bool IsFalling;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation parameters")
		bool IsDriving;

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (BlueprintThreadSafe))
		float GetHorizontalVelocityMagnitude() const;

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY()
		ACharacter* character;
	UPROPERTY()
		UAxisMovement* axisMovement;

	void SetVelocity();
	void SetLegState();
	void SetIsFalling();
};
