#pragma once
#include "CIVEditorSettings.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE_OneParam(FOnSettingsChanged, UCIVEditorSettings*);

class SCIVSettingsEditWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCIVSettingsEditWidget) {}
		SLATE_EVENT(FOnSettingsChanged, OnSettingsChangedDelegate)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual ~SCIVSettingsEditWidget() = default;

	FOnSettingsChanged OnSettingsChanged;
};
