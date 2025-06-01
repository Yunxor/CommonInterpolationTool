#pragma once

#include "CIVEditorSettings.h"
#include "Widgets/SCompoundWidget.h"

class SCIVCommonCurvePainter;

DECLARE_DELEGATE_RetVal_TwoParams(float, FOnRefreshUpdateFunction, float /* DeltaTime */, float /* TargetValue */);
DECLARE_DELEGATE_OneParam(FResetDataFunction, float /* NewSource */);

class COMMONINTERPOLATIONVISUALIZER_API SCIVCommonInterpWidget : public SCompoundWidget
{
	
public:
	SLATE_BEGIN_ARGS(SCIVCommonInterpWidget)
		{}
		SLATE_EVENT(FOnRefreshUpdateFunction, UpdateDelegate)
		SLATE_EVENT(FResetDataFunction, ResetDelegate)
	SLATE_END_ARGS();

	void OnSettingsChanged(UCIVEditorSettings* EditorSettings);
	void Construct(const FArguments& InArgs);

	virtual ~SCIVCommonInterpWidget() = default;
	
	void OnIntervalChanged(float NewInterval);
	
	void Reset(FOnRefreshUpdateFunction& Predicate, float InInterval = 0.01f, bool bRefresh = true);

	void RefreshGraph();


protected:
	// child panel：parameter input widget.
	TSharedPtr<SWidget> ParameterPanel;

private:
	TArray<FVector2D> Samples;

	FOnRefreshUpdateFunction UpdateFunction;
	FResetDataFunction ResetDataFunction;
};
