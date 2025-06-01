#pragma once

#include "EditorUtilityWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "SpringInterpValueWidget.generated.h"

UCLASS()
class SPRINGINTERPVISUALIZER_API USpringInterpValueWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
public:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	void OnValueChanged(float NewValue);
	void OnResetSampleData(float NewSource);
	float OnUpdateSampleData(float DeltaTime, float TargetValue);
private:
	TSharedPtr<class SCIVCommonInterpWidget> WidgetPtr;
	
	// settings.
	float Stiffness = 0;
	float CriticalDamping = 0;
	float Mass = 0;
	float TargetVelocityAmount = 0;
	bool bClamp = false;
	float MinValue = 0;
	float MaxValue = 0;
	bool bInitializeFromTarget = false;


	FFloatSpringState SpringState;
	float CurrentValue = 0;
	
};
