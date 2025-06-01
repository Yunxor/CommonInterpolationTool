// Fill out your copyright notice in the Description page of Project Settings.


#include "CIVCommonCurvePainter.h"

#define LOCTEXT_NAMESPACE "SCIVCommonCurvePainter"

void SCIVCommonCurvePainter::Construct(const FArguments& InArgs)
{
	Atr_HorizontalMax = InArgs._Horizontal;
	Atr_VerticalMax = InArgs._Vertical;
	Atr_Samples = InArgs._Samples;
	Atr_UseCustomTarget = InArgs._UseCustomTarget;
	Atr_CustomTargetSamples = InArgs._CustomTargetSamples;
	Atr_InterpTarget = InArgs._InterpTarget;
}

int32 SCIVCommonCurvePainter::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const float Pad = AllottedGeometry.Size.Y * 0.1f;
	
	const float VerticalBottom = AllottedGeometry.Size.Y - Pad;
	
	const float CurveHeight = AllottedGeometry.Size.Y - Pad * 2.f;
	const float CurveWidth = AllottedGeometry.Size.X - Pad;

	const float TextOffsetX = Pad * 0.5f;

	/* (0,0) -------|-------------------------- (Size.X, 0)
	 * |			|		Pad
	 * | -----------||========================== 
	 * |			||		↑
	 * |  	Pad		||	CurveHeight
	 * |			||[<- 	↓ 	CurveWidth  ->]
	 * | -----------||========================= (Size.X, VerticalBottom)
	 * |			|		Pad
	 * (0, Size.Y)--|--------------------------
	 */

	// Draw limit line.
	TArray<FVector2D> Points;
	Points.Add(FVector2D(Pad, Pad));
	Points.Add(FVector2D(Pad, VerticalBottom));
	Points.Add(FVector2D(CurveWidth, VerticalBottom));
	 

	FSlateDrawElement::MakeLines(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(),
		Points,
		ESlateDrawEffect::None);

	
	// Draw limit text.
	const FSlateFontInfo SmallLayoutFont = FCoreStyle::GetDefaultFontStyle("Regular", 8);
	
	// print text of original point.
	FVector2D TextDrawPosition = FVector2D(TextOffsetX, VerticalBottom);
	FSlateDrawElement::MakeText(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(AllottedGeometry.Size, FSlateLayoutTransform(TextDrawPosition)),
		"0",
		SmallLayoutFont);

	const float& HorizontalMaxValue = Atr_HorizontalMax.Get();
	
	// print text of horizontal max value.
	TextDrawPosition = FVector2D(AllottedGeometry.Size.X - TextOffsetX, VerticalBottom);
	FSlateDrawElement::MakeText(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(AllottedGeometry.Size, FSlateLayoutTransform(TextDrawPosition)),
		FString::SanitizeFloat(HorizontalMaxValue),
		SmallLayoutFont);

	
	const float& VerticalMaxValue = Atr_VerticalMax.Get();
	// print text of vertical max value.
	TextDrawPosition = FVector2D(TextOffsetX, Pad);
	
	FSlateDrawElement::MakeText(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(AllottedGeometry.Size, FSlateLayoutTransform(TextDrawPosition)),
		FString::SanitizeFloat(VerticalMaxValue),
		SmallLayoutFont);

	
	if (!Atr_UseCustomTarget.Get())
	{
		const float& TargetValue = Atr_InterpTarget.Get();
		const float TargetHeight = (1 - TargetValue / VerticalMaxValue) * CurveHeight + Pad;
		// print text of Interp target value.
		TextDrawPosition = FVector2D(TextOffsetX, TargetHeight);
		FSlateDrawElement::MakeText(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(AllottedGeometry.Size, FSlateLayoutTransform(TextDrawPosition)),
			FString::SanitizeFloat(TargetValue),
			SmallLayoutFont);

		
		Points.Reset(2);
		
		Points.Add(FVector2D(Pad, TargetHeight));
		Points.Add(FVector2D(CurveWidth, TargetHeight));
		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			Points,
			ESlateDrawEffect::None);
	}
	else
	{
		// Draw custom curve.
		const TArray<FVector2D>& Samples = Atr_CustomTargetSamples.Get();
		Points.Reset(Samples.Num());
		FVector2D Offset(Pad, VerticalBottom);
		for (FVector2D Sample : Samples)
		{
			Points.Add(Offset + FVector2D(
				CurveWidth * Sample.X / HorizontalMaxValue,
				-CurveHeight * Sample.Y / VerticalMaxValue
					   ));
		}

		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			Points,
			ESlateDrawEffect::None,
			FLinearColor::White);
	}

	
	// Draw evaluate curve.
	const TArray<FVector2D>& Samples = Atr_Samples.Get();
	Points.Reset(Samples.Num());
	FVector2D Offset(Pad, VerticalBottom);
	for (FVector2D Sample : Samples)
	{
		Points.Add(Offset + FVector2D(
			CurveWidth * Sample.X / HorizontalMaxValue,
			-CurveHeight * Sample.Y / VerticalMaxValue
		           ));
	}

	FSlateDrawElement::MakeLines(
		OutDrawElements,
		LayerId + 1,
		AllottedGeometry.ToPaintGeometry(),
		Points,
		ESlateDrawEffect::None,
		FLinearColor::Green);


	return LayerId + 1;
}

FVector2D SCIVCommonCurvePainter::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return FVector2D(200.f, 200.f);
}
#undef LOCTEXT_NAMESPACE
