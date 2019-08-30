// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "TimelineCurveBase.h"

/**
 *
 */
class UNREALTESTPROJECT_API MyUtils
{
public:
	MyUtils();
	~MyUtils();

	static UTimelineComponent* CreateTimeline(AActor* owner, FName name, TArray<UTimelineCurveBase*> curves);
	static void LogError(FString message);
};
