// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RequireInput.h"
#include "MyCar.h"
#include "InteractComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALTESTPROJECT_API UInteractComponent : public UActorComponent, public IRequireInput
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Interact component methods")
		void EndInteract() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact component methods")
		void InitializeInput(UInputComponent* inputComponent);
	virtual void InitializeInput_Implementation(UInputComponent* inputComponent) override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
		ACharacter* ownerCharacter;

	UPROPERTY()
		TScriptInterface<IInteractable> triggeredInteractable = TScriptInterface<IInteractable>();
	UPROPERTY()
		TScriptInterface<IInteractable> targetInteractable = TScriptInterface<IInteractable>();

	UFUNCTION()
		void ActorBeginOverlap(AActor* thisActor, AActor* otherActor);
	UFUNCTION()
		void ActorEndOverlap(AActor* thisActor, AActor* otherActor);

	void StopInteracting();
	void ReceiveAxisInput(float axisValues);
	void Interact();
};
