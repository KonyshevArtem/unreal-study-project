// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RequireInput.h"
#include "MyCar.h"
#include "DrivingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALTESTPROJECT_API UDrivingComponent : public UActorComponent, public IRequireInput
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDrivingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
		bool HasReachedCarEnter() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Driving component methods")
		void InitializeInput(UInputComponent* inputComponent);
	virtual void InitializeInput_Implementation(UInputComponent* inputComponent) override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
		AMyCar* triggeredCar;
	UPROPERTY()
		bool isEnteringCar;
	UPROPERTY()
		USceneComponent* carEnterPoint;
	UPROPERTY()
		ACharacter* ownerCharacter;
	UPROPERTY()
		AController* ownerController;
	UPROPERTY()
		AMyCar* targetCar;
	
	UFUNCTION()
		void ActorBeginOverlap(AActor* thisActor, AActor* otherActor);
	UFUNCTION()
		void ActorEndOverlap(AActor* thisActor, AActor* otherActor);
	
	void StopGoingToCar();
	void StopGoingToCar(float axisValues);
	void GoToCar();
	USceneComponent* GetClosestEnterPoint(AMyCar* car);
	float GetDistanceToEnterPoint(USceneComponent* enterPoint);
};
