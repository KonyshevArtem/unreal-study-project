// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimelineCurveBase.h"
#include "Curves/CurveVector.h"
#include "Components/TimelineComponent.h"
#include "TimelineCurveVector.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTESTPROJECT_API UTimelineCurveVector : public UTimelineCurveBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		UCurveVector* CurveVector;
	UPROPERTY()
		FOnTimelineVector CurveCallback;
	
	~UTimelineCurveVector() = default;
};
