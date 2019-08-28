// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimelineCurveBase.h"
#include "Curves/CurveLinearColor.h"
#include "Components/TimelineComponent.h"
#include "TimelineCurveLinearColor.generated.h"

/**
 *
 */
UCLASS()
class UNREALTESTPROJECT_API UTimelineCurveLinearColor : public UTimelineCurveBase
{
	GENERATED_BODY()

public:
	UPROPERTY()
		UCurveLinearColor* CurveLinearColor;
	UPROPERTY()
		FOnTimelineLinearColor CurveCallback;

	~UTimelineCurveLinearColor() = default;
};
