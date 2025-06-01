#include "SpringInterpValueWidget.h"

#include "CommonInterpolationVisualizer/Public/CIVCommonInterpWidget.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SNumericEntryBox.h"


TSharedRef<SWidget> USpringInterpValueWidget::RebuildWidget()
{

	TSharedPtr<SVerticalBox> InputBox = SNew(SVerticalBox);
	
	auto AddInputBox = [this, InputBox](const FString& ShowText, float& BindingValue)
	{
		InputBox->AddSlot()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString(ShowText))
			]
			+SHorizontalBox::Slot()
			.FillWidth(1.f)
			[
				SNew(SSpinBox<float>)
				.Value(BindingValue)
				.OnValueCommitted_Lambda([this, &BindingValue](float NewValue, ETextCommit::Type InCommitType) { BindingValue = NewValue; })
				.OnValueChanged_UObject(this, &USpringInterpValueWidget::OnValueChanged)
			]
		];
	};

	AddInputBox( "Stiffness", this->Stiffness);
	AddInputBox( "CriticalDamping", this->CriticalDamping);
	AddInputBox( "Mass", this->Mass);
	AddInputBox( "TargetVelocityAmount", this->TargetVelocityAmount);

	auto ClampInputEnableLambda = [this]()
	{
		return this->bClamp;
	};
	
	InputBox->AddSlot()
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock).Text(FText::FromString("Clamp:"))
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SCheckBox)
			.IsChecked_Lambda([this]()
			{
				return bClamp ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
			})
			.OnCheckStateChanged_Lambda([this](ECheckBoxState NewState)
			{
				this->bClamp = NewState == ECheckBoxState::Checked;
				this->OnValueChanged(true);
			})
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock).Text(FText::FromString("Min:"))
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SSpinBox<float>)
			.IsEnabled_Lambda(ClampInputEnableLambda)
			.Value(this->MinValue)
			.MaxValue(this->MaxValue)
			.OnValueCommitted_Lambda([this](double InValue, ETextCommit::Type InCommitType){ MinValue = InValue; })
			.OnValueChanged_UObject(this, &USpringInterpValueWidget::OnValueChanged)
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock).Text(FText::FromString("Max:"))
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SSpinBox<float>)
			.IsEnabled_Lambda(ClampInputEnableLambda)
			.Value(this->MaxValue)
			.MinValue(this->MinValue)
			.OnValueCommitted_Lambda([this](double InValue, ETextCommit::Type InCommitType){ MaxValue = InValue; })
			.OnValueChanged_UObject(this, &USpringInterpValueWidget::OnValueChanged)
		]
	];

	InputBox->AddSlot()
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock).Text(FText::FromString("bInitializeFromTarget:"))
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SCheckBox)
			.IsChecked_Lambda([this]()
			{
				return bInitializeFromTarget ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
			})
			.OnCheckStateChanged_Lambda([this](ECheckBoxState NewState)
			{
				this->bInitializeFromTarget = NewState == ECheckBoxState::Checked;
				this->OnValueChanged(true);
			})
		]
	];

	TSharedRef<SVerticalBox> Box  =
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5.f)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
			.Padding(FMargin(10.f)) // Inner distance
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 0, 0, 5)
				[
					SNew(STextBlock).Text(FText::FromString("Spring Interp Params"))
					.Font(FAppStyle::GetFontStyle("HeadingExtraSmall"))
					.Justification(ETextJustify::Center)
				]
				+SVerticalBox::Slot()
				.FillHeight(1.f)
				[
					InputBox.ToSharedRef()
				]
			]
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SImage)
			.Image(FAppStyle::GetBrush("WhiteBrush"))
			.ColorAndOpacity(FLinearColor::Gray)
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(2)
		[
			SAssignNew(WidgetPtr, SCIVCommonInterpWidget)
			.ResetDelegate_UObject(this, &USpringInterpValueWidget::OnResetSampleData)
			.UpdateDelegate_UObject(this, &USpringInterpValueWidget::OnUpdateSampleData)
		];
		
	return Box;
}

void USpringInterpValueWidget::OnValueChanged(float NewValue)
{
	WidgetPtr->RefreshGraph();
}

void USpringInterpValueWidget::OnResetSampleData(float NewSource)
{
	CurrentValue = 0.f;
	SpringState.Reset();
}

float USpringInterpValueWidget::OnUpdateSampleData(float DeltaTime, float TargetValue)
{
	CurrentValue = UKismetMathLibrary::FloatSpringInterp(CurrentValue, TargetValue, SpringState, Stiffness, CriticalDamping, DeltaTime, Mass, TargetVelocityAmount,
		bClamp, MinValue, MaxValue, bInitializeFromTarget);

	return CurrentValue;
}
