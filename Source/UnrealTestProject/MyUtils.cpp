// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUtils.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Runtime/Core/Public/Containers/Array.h"
#include "TimelineCurveFloat.h"
#include "TimelineCurveVector.h"
#include "TimelineCurveLinearColor.h"

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
		if (Cast<UTimelineCurveFloat>(curveInfo) != nullptr)
		{
			UTimelineCurveFloat* floatCurveInfo = Cast<UTimelineCurveFloat>(curveInfo);
			timeline->AddInterpFloat(floatCurveInfo->CurveFloat, floatCurveInfo->CurveCallback);
		}
		else if (Cast<UTimelineCurveVector>(curveInfo) != nullptr)
		{
			UTimelineCurveVector* vectorCurveInfo = Cast<UTimelineCurveVector>(curveInfo);
			timeline->AddInterpVector(vectorCurveInfo->CurveVector, vectorCurveInfo->CurveCallback);
		}
		else if (Cast<UTimelineCurveLinearColor>(curveInfo) != nullptr)
		{
			UTimelineCurveLinearColor* linearColorCurveInfo = Cast<UTimelineCurveLinearColor>(curveInfo);
			timeline->AddInterpLinearColor(linearColorCurveInfo->CurveLinearColor, linearColorCurveInfo->CurveCallback);
		}
	}

	timeline->RegisterComponent();
	return timeline;
}
