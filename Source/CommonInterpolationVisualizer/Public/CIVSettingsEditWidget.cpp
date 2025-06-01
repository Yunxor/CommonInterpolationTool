#include "CIVSettingsEditWidget.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "CIVEditorSettings.h"
#include "PropertyCustomizationHelpers.h"

void SCIVSettingsEditWidget::Construct(const FArguments& InArgs)
{
	OnSettingsChanged = InArgs._OnSettingsChangedDelegate;
	
	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(2.f)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Interval:"))
			]
			+SHorizontalBox::Slot()
			.FillWidth(1.f)
			[
				SNew(SSpinBox<float>)
				.MinValue(0.01f)
				.MaxValue(10.f)
				.Value_Lambda([]()
				{
					UCIVEditorSettings* Settings = GetMutableDefault<UCIVEditorSettings>();
					check(Settings)
					return Settings->IntervalTime;
				})
				.OnValueChanged_Lambda([this](float NewValue)
				{
					UCIVEditorSettings* Settings = GetMutableDefault<UCIVEditorSettings>();
					check(Settings)
					Settings->IntervalTime = NewValue;
					Settings->SaveConfig();
					OnSettingsChanged.ExecuteIfBound(Settings);
				})
			]
		]
		

		+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(2.f)
		[
			// default settings.
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(2.f)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Max X: "))
				.ToolTipText(FText::FromString("Max X value (max time \\ max horizontal value)"))
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(SSpinBox<float>)
				.MinValue(0.01f)
				.Value_Lambda([]()
				{
					UCIVEditorSettings* Settings = GetMutableDefault<UCIVEditorSettings>();
					check(Settings)
					return Settings->MaxHorizontalValue;
				})
				.OnValueChanged_Lambda([this](float NewValue)
				{
					UCIVEditorSettings* Settings = GetMutableDefault<UCIVEditorSettings>();
					check(Settings)
					Settings->MaxHorizontalValue = NewValue;
					Settings->SaveConfig();
					OnSettingsChanged.ExecuteIfBound(Settings);
				})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Max Y: "))
				.ToolTipText(FText::FromString("Max Y Value (max value \\ max vertical value)"))
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(SSpinBox<float>)
				.MinValue(0.01f)
				.MaxValue(10.f)
				.Value_Lambda([]()
				{
					UCIVEditorSettings* Settings = GetMutableDefault<UCIVEditorSettings>();
					check(Settings)
					return Settings->MaxVerticalValue;
				})
				.OnValueChanged_Lambda([this](float NewValue)
				{
					UCIVEditorSettings* Settings = GetMutableDefault<UCIVEditorSettings>();
					check(Settings)
					Settings->MaxVerticalValue = NewValue;
					Settings->SaveConfig();
					OnSettingsChanged.ExecuteIfBound(Settings);
				})
			]
		]
		
		+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(2.f)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString("EnableCustomTargetCurve: "))
			]
			+SHorizontalBox::Slot()
			.FillWidth(1.f)
			[
				SNew(SCheckBox)
				.IsChecked_Lambda([]()
				{
					UCIVEditorSettings* Settings = GetMutableDefault<UCIVEditorSettings>();
					check(Settings)
					return Settings->bEnableCustomTargetCurve ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
				})
				.OnCheckStateChanged_Lambda([this](ECheckBoxState NewState)
				{
					UCIVEditorSettings* Settings = GetMutableDefault<UCIVEditorSettings>();
					check(Settings)
					Settings->bEnableCustomTargetCurve = (NewState == ECheckBoxState::Checked);
					Settings->SaveConfig();
					OnSettingsChanged.ExecuteIfBound(Settings);
				})
			]
		]
		
		+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(2.f)
		[
			// default settings.
			SNew(SHorizontalBox)
			.Visibility_Lambda(
			[]()
			{
				UCIVEditorSettings* Settings = GetMutableDefault<UCIVEditorSettings>();
				check(Settings)
				return Settings->bEnableCustomTargetCurve ? EVisibility::Collapsed : EVisibility::Visible;
			})
			// Target Value:
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(2.f)
			[
				SNew(STextBlock)
				.Text(FText::FromString("T: "))
				.ToolTipText(FText::FromString("Interp Target Value"))
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(SSpinBox<float>)
				.Value_Lambda([]()
				{
					UCIVEditorSettings* Settings = GetMutableDefault<UCIVEditorSettings>();
					check(Settings)
					return Settings->InterpTargetValue;
				})
				.OnValueChanged_Lambda([this](float NewValue)
				{
					UCIVEditorSettings* Settings = GetMutableDefault<UCIVEditorSettings>();
					check(Settings)
					Settings->InterpTargetValue = NewValue;
					Settings->SaveConfig();
					OnSettingsChanged.ExecuteIfBound(Settings);
				})
			]
		]
		
		// Custom curve.
		+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(2.f)
		[
			SNew(SHorizontalBox)
			.Visibility_Lambda(
			[]()
			{
				UCIVEditorSettings* Settings = GetMutableDefault<UCIVEditorSettings>();
				check(Settings)
				return Settings->bEnableCustomTargetCurve ? EVisibility::Visible : EVisibility::Collapsed;
			})
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString("CustomTargetCurve:"))
			]
			+SHorizontalBox::Slot()
			.FillWidth(1.f)
			[
				SNew(SObjectPropertyEntryBox)
				.ObjectPath_Lambda([]()
				{
					UCIVEditorSettings* Settings = GetMutableDefault<UCIVEditorSettings>();
					check(Settings)
					return Settings->TargetValueCurvePath.ToString();
				})
				.AllowedClass(UCurveFloat::StaticClass())
				.OnObjectChanged_Lambda([this](const FAssetData& AssetData)
				{
					UCIVEditorSettings* Settings = GetMutableDefault<UCIVEditorSettings>();
					check(Settings)
					Settings->TargetValueCurvePath = AssetData.ToSoftObjectPath();
					Settings->SaveConfig();
					OnSettingsChanged.ExecuteIfBound(Settings);
				})
			]
		]
	];
}
