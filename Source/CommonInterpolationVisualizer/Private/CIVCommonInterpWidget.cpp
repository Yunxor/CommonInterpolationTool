#include "CIVCommonInterpWidget.h"
#include "CIVCommonCurvePainter.h"
#include "CIVEditorSettings.h"
#include "CIVSettingsEditWidget.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "CIVCommonInterpWidget"

void SCIVCommonInterpWidget::Construct(const FArguments& InArgs)
{
	UpdateFunction = InArgs._UpdateDelegate;
	ResetDataFunction = InArgs._ResetDelegate;

	RefreshGraph();

	ChildSlot
	[
		SNew(SVerticalBox)
		// Draw Input box.
		+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5.f)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 0, 0, 5)
				[
					SNew(STextBlock).Text(FText::FromString("CIV Editor Settings"))
					.Font(FAppStyle::GetFontStyle("HeadingExtraSmall"))
					.Justification(ETextJustify::Center)
				]
				+SVerticalBox::Slot()
				.FillHeight(1.f)
				[
					SNew(SCIVSettingsEditWidget)
					.OnSettingsChangedDelegate_Raw(this, &SCIVCommonInterpWidget::OnSettingsChanged)
				]
			]
		]
		// Draw curve.
		+SVerticalBox::Slot()
		.Padding(5.f)
		.FillHeight(1.f)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 0, 0, 5)
				[
					SNew(STextBlock).Text(FText::FromString("CIV Common Curve Painter"))
					.Font(FAppStyle::GetFontStyle("HeadingExtraSmall"))
					.Justification(ETextJustify::Center)
				]
				+SVerticalBox::Slot()
				.FillHeight(1.f)
				[
					SNew(SCIVCommonCurvePainter)
					.Samples_Lambda([this]() { return Samples; })
					.Horizontal_Lambda([this]()
					{
						const UCIVEditorSettings* Settings = GetDefault<UCIVEditorSettings>();
						return Settings ? Settings->MaxHorizontalValue : 2.f;
					})
					.Vertical_Lambda([this]()
					{
						const UCIVEditorSettings* Settings = GetDefault<UCIVEditorSettings>();
						return Settings ? Settings->MaxVerticalValue : 2.f;
					})
					.UseCustomTarget_Lambda([]()
					{
						const UCIVEditorSettings* Settings = GetDefault<UCIVEditorSettings>();
						return Settings ? Settings->bEnableCustomTargetCurve : false;
					})
					.CustomTargetSamples_Lambda([]()
					{
						if (const UCIVEditorSettings* Settings = GetDefault<UCIVEditorSettings>())
						{
							return Settings->GetTargetCurveSamples(Settings->MaxHorizontalValue, Settings->IntervalTime);
						}
						return TArray<FVector2D>();
					})
					.InterpTarget_Lambda([]()
					{
						if (const UCIVEditorSettings* Settings = GetDefault<UCIVEditorSettings>())
						{
							return Settings->InterpTargetValue;
						}
						return 1.f;
					})
				]

			]
		]
	];
}

void SCIVCommonInterpWidget::OnSettingsChanged(UCIVEditorSettings* EditorSettings)
{
	RefreshGraph();
}

void SCIVCommonInterpWidget::OnIntervalChanged(float NewInterval)
{
	RefreshGraph();
}

void SCIVCommonInterpWidget::Reset(FOnRefreshUpdateFunction& Predicate, float InInterval, bool bRefresh)
{
	UpdateFunction = Predicate;

	if (bRefresh)
	{
		RefreshGraph();
	}
}

void SCIVCommonInterpWidget::RefreshGraph()
{
	if (!UpdateFunction.IsBound())
	{
		UE_LOG(LogTemp, Error, TEXT("[SCIVCommonInterpWidget::RefreshGraph]: UpdateFunction is null."));
	}
	Samples.Reset();
	if (ResetDataFunction.IsBound())
	{
		ResetDataFunction.Execute(0.f);
	}
	const UCIVEditorSettings* Settings = GetDefault<UCIVEditorSettings>();
	check(Settings)

	const float IntervalTime = Settings->IntervalTime;
	const float MaxHorizontalValue = Settings->MaxHorizontalValue;
	const bool bEnableCustomTargetCurve = Settings->bEnableCustomTargetCurve;
	const UCurveFloat* FloatCurve = Settings->GetTargetCurve();
	if (bEnableCustomTargetCurve && FloatCurve)
	{
		float Interp = 0.f;
		while (Interp <= MaxHorizontalValue)
		{
			const float TargetValue = FloatCurve->GetFloatValue(Interp);
			Samples.Add(FVector2D( (Interp), UpdateFunction.Execute(IntervalTime, TargetValue)));
			Interp += IntervalTime;
		}
	}
	else
	{
		float Interp = 0.f;
		while (Interp <= MaxHorizontalValue)
		{
			Samples.Add(FVector2D(Interp, UpdateFunction.Execute(IntervalTime, Settings->InterpTargetValue)));
			Interp += IntervalTime;
		}
	}
}

#undef LOCTEXT_NAMESPACE
