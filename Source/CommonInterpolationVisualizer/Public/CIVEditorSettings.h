// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "UObject/SoftObjectPath.h"
#include "CIVEditorSettings.generated.h"

/**
 * 
 */
UCLASS(config = Editor, defaultconfig, meta = (DisplayName = "Common Interp Visualizer"))
class COMMONINTERPOLATIONVISUALIZER_API UCIVEditorSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(Config, EditAnywhere, Category=Settings)
	float IntervalTime = 0.01f;

	UPROPERTY(Config, EditAnywhere, Category=Settings)
	bool bEnableCustomTargetCurve = false;

	UPROPERTY(Config, EditAnywhere, Category=Settings)
	float MaxHorizontalValue = 2.f;
	
	UPROPERTY(Config, EditAnywhere, Category=Settings)
	float MaxVerticalValue = 2.f;
	
	UPROPERTY(Config, EditAnywhere, Category=Settings, meta=(EditCondition="!bEnableCustomTargetCurve"))
	float InterpTargetValue = 1.f;
	
	UPROPERTY(Config, EditAnywhere, Category=Settings, meta=(EditCondition="bEnableCustomTargetCurve"))
	FSoftObjectPath TargetValueCurvePath;

public:
	UCurveFloat* GetTargetCurve() const
	{
		if (TargetValueCurvePath.IsValid() && !TargetValueCurvePath.IsNull())
		{
			return Cast<UCurveFloat>(TargetValueCurvePath.TryLoad());
		}
		return nullptr;
	}

	TArray<FVector2D> GetTargetCurveSamples(const float MaxTime, const float SampleInterval) const
	{
		TArray<FVector2D> Samples;
		if (const UCurveFloat* Curve = GetTargetCurve())
		{
			for (float Time = 0; Time <= MaxTime; Time += SampleInterval)
			{
				Samples.Add(FVector2D{Time, Curve->GetFloatValue(Time)});
			}
		}
		return Samples;
	}
};
