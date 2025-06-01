// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SLeafWidget.h"

/**
 * 
 */


class COMMONINTERPOLATIONVISUALIZER_API SCIVCommonCurvePainter : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SCIVCommonCurvePainter)
		: _Samples({FVector2D(0.f, 1.f), FVector2D(0.f, 1.f)})
		, _Horizontal(1.f)
		, _Vertical(2.f)
		, _InterpTarget(1.f)
	{}
		SLATE_ATTRIBUTE(TArray<FVector2D>, Samples)
		SLATE_ATTRIBUTE(float, Horizontal)
		SLATE_ATTRIBUTE(float, Vertical)
		SLATE_ATTRIBUTE(float, InterpTarget)
		SLATE_ATTRIBUTE(TArray<FVector2D>, CustomTargetSamples)
		SLATE_ATTRIBUTE(bool, UseCustomTarget)
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);
	
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	
private:
	TAttribute<TArray<FVector2D>> Atr_Samples;
	TAttribute<TArray<FVector2D>> Atr_CustomTargetSamples;
	TAttribute<bool> Atr_UseCustomTarget;
	TAttribute<float> Atr_HorizontalMax;
	TAttribute<float> Atr_VerticalMax;
	TAttribute<float> Atr_InterpTarget;
};
