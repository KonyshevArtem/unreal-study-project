// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUtils.h"
#include "Engine/Engine.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Runtime/Core/Public/Containers/Array.h"
#include "TimelineCurveFloat.h"
#include "TimelineCurveVector.h"
#include "TimelineCurveLinearColor.h"
#include <limits>

MyUtils::MyUtils()
{
}

MyUtils::~MyUtils()
{
}

UTimelineComponent* MyUtils::CreateTimeline(AActor* owner, FName name, TArray<UTimelineCurveBase*> curves)
{
	UObject* outer = Cast<UObject>(owner);
	UTimelineComponent* timeline = NewObject<UTimelineComponent>(outer, name);
	timeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	timeline->SetNetAddressable();
	owner->BlueprintCreatedComponents.Add(timeline);
	timeline->SetPropertySetObject(outer);

	for (UTimelineCurveBase* curveInfo : curves)
	{
		if (UTimelineCurveFloat* floatCurveInfo = Cast<UTimelineCurveFloat>(curveInfo))
		{
			timeline->AddInterpFloat(floatCurveInfo->CurveFloat, floatCurveInfo->CurveCallback);
		}
		else if (UTimelineCurveVector* vectorCurveInfo = Cast<UTimelineCurveVector>(curveInfo))
		{
			timeline->AddInterpVector(vectorCurveInfo->CurveVector, vectorCurveInfo->CurveCallback);
		}
		else if (UTimelineCurveLinearColor* linearColorCurveInfo = Cast<UTimelineCurveLinearColor>(curveInfo))
		{
			timeline->AddInterpLinearColor(linearColorCurveInfo->CurveLinearColor, linearColorCurveInfo->CurveCallback);
		}
	}

	timeline->RegisterComponent();
	return timeline;
}

void MyUtils::LogError(FString message)
{
	UE_LOG(LogTemp, Error, TEXT("%s"), *message);
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "[Error] " + message);
}

template <typename T>
T MyUtils::Find(TArray<T> arr, TFunction<bool(T, T)> comparator)
{
	if (arr.Num() == 0) return nullptr;
	if (arr.Num() == 1) return arr[0];

	T target = arr[0];
	for (int i = 1; i < arr.Num(); ++i)
	{
		if (comparator(target, arr[i]))
		{
			target = arr[i];
		}
	}
	return target;
}

