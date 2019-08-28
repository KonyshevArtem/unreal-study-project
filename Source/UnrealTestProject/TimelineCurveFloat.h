// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimelineCurveBase.h"
#include "Curves/CurveFloat.h"
#include "Components/TimelineComponent.h"
#include "TimelineCurveFloat.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTESTPROJECT_API UTimelineCurveFloat : public UTimelineCurveBase
{
	GENERATED_BODY()

public:
	UPROPERTY()
		UCurveFloat* CurveFloat;
	UPROPERTY()
		FOnTimelineFloat CurveCallback;

	~UTimelineCurveFloat() = default;
};
