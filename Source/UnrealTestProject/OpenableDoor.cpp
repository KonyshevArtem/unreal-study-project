// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenableDoor.h"
#include "Engine/Engine.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "MyUtils.h"
#include "TimelineCurveFloat.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "MyCharacter.h"

// Sets default values
AOpenableDoor::AOpenableDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOpenableDoor::BeginPlay()
{
	Super::BeginPlay();

	if (!InnerTrigger || !OuterTrigger)
	{
		MyUtils::LogError("Openable door's triggers not set");
		return;
	}
	InnerTrigger->OnComponentBeginOverlap.AddDynamic(this, &AOpenableDoor::OnTriggerBeginOverlap);
	OuterTrigger->OnComponentBeginOverlap.AddDynamic(this, &AOpenableDoor::OnTriggerBeginOverlap);

	rotationMultiplierToTrigger = TMap<UBoxComponent*, int>();
	rotationMultiplierToTrigger.Add(InnerTrigger, 1);
	rotationMultiplierToTrigger.Add(OuterTrigger, -1);

	InitOpenDoorTimeline();
}

// Called every frame
void AOpenableDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetIsOpened();
	if (!openDoorTimeline) return;
	openDoorTimeline->TickComponent(DeltaTime, LEVELTICK_TimeOnly, nullptr);
}

void AOpenableDoor::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult)
{
	if (IsOverlapping) return;
	IsOverlapping = true;
	if (IsOpened) return;
	
	rotationMultiplier = rotationMultiplierToTrigger[Cast<UBoxComponent>(OverlappedComp)];
	GetWorldTimerManager().SetTimer(openDoorDelayHandle, this, &AOpenableDoor::OpenDoor, 0.2f);
	if (AMyCharacter * myCharacter = Cast<AMyCharacter>(OtherActor))
	{
		myCharacter->OpenDoor();
	}
}

void AOpenableDoor::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	IsOverlapping = false;
}

void AOpenableDoor::SetIsOpened()
{
	IsOpened = FMath::Abs(Door->GetRelativeTransform().Rotator().Yaw) >= 90;
}

void AOpenableDoor::SetDoorRotation(float interpolatedVal)
{
	const FRotator currentRotation = Door->GetRelativeTransform().Rotator();
	const FRotator newRotation(0, interpolatedVal * appliedRotationMultiplier, 0);
	Door->SetRelativeRotation(FMath::Lerp(currentRotation, newRotation, GetWorld()->GetDeltaSeconds() * 20));
}

void AOpenableDoor::InitOpenDoorTimeline()
{
	if (!DoorRotationZCurve)
	{
		MyUtils::LogError("Door rotation z curve not set");
		return;
	}
	FOnTimelineFloat onTimelineCallback;
	onTimelineCallback.BindUFunction(this, FName("SetDoorRotation"));
	UTimelineCurveFloat* openDoorCurveInfo = NewObject<UTimelineCurveFloat>();
	openDoorCurveInfo->CurveFloat = DoorRotationZCurve;
	openDoorCurveInfo->CurveCallback = onTimelineCallback;
	TArray<UTimelineCurveBase*> curves;
	curves.Add(openDoorCurveInfo);
	openDoorTimeline = MyUtils::CreateTimeline(this, FName("OpenDoor"), curves);
}

void AOpenableDoor::OpenDoor()
{
	appliedRotationMultiplier = rotationMultiplier;
	if (!openDoorTimeline) return;
	openDoorTimeline->PlayFromStart();
}

