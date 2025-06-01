#include "SpringInterpVisualizer.h"

#include "LevelEditor.h"
#include "SpringInterpValueWidget.h"
#include "Interfaces/IMainFrameModule.h"

#define LOCTEXT_NAMESPACE "FSpringInterpVisualizerModule"

void FSpringInterpVisualizer::StartupModule()
{
	// Get Main menu module.
    FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	// Create menu Extender.
	MenuExtender = MakeShareable(new FExtender());
	MenuExtender->AddMenuExtension(
		"WindowLayout",
		EExtensionHook::After,
		nullptr,
		FMenuExtensionDelegate::CreateRaw(this, &FSpringInterpVisualizer::AddMenuEntry)
		);

	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
}

void FSpringInterpVisualizer::ShutdownModule()
{
	if (FLevelEditorModule* LevelEditorModule = FModuleManager::GetModulePtr<FLevelEditorModule>("LevelEditor"))
	{
		LevelEditorModule->GetMenuExtensibilityManager()->RemoveExtender(MenuExtender);
	}
}

void FSpringInterpVisualizer::AddMenuEntry(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(
		FText::FromString("Spring Interp Value Visualizer"),
		FText::FromString("Show Spring interp values ."),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.PlacementBrowser"),
		FUIAction(FExecuteAction::CreateRaw(this, &FSpringInterpVisualizer::OpenCustomEditorWindow))
		);
}

void FSpringInterpVisualizer::OpenCustomEditorWindow()
{
	if (CustomWindowInstance.IsValid())
	{
		CustomWindowInstance.Pin()->BringToFront();
		return;
	}

	 TSharedRef<SWindow> CustomWindow = SNew(SWindow)
	.Title(FText::FromString("Spring Interp Visualizer"))
	.ClientSize(FVector2D(800, 600))
	.SupportsMinimize(true)
	.SupportsMaximize(true);

	UWorld* World = GEditor->GetEditorWorldContext().World();
	USpringInterpValueWidget* ValueWidget = CreateWidget<USpringInterpValueWidget>(World);
	TSharedPtr<SWidget> ValueWidgetPtr = ValueWidget->TakeWidget();
	
	CustomWindow->SetContent(ValueWidgetPtr.ToSharedRef());


	// Add to editor as modal window or independent window.
	IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
	if (MainFrameModule.GetParentWindow().IsValid())
	{
		// Added as editor sub window.
		FSlateApplication::Get().AddWindowAsNativeChild(CustomWindow, MainFrameModule.GetParentWindow().ToSharedRef());
	}
	else
	{
		// Independent window ( When the editor is not started)
		FSlateApplication::Get().AddWindow(CustomWindow);
	}

	CustomWindowInstance = CustomWindow;
}


#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FSpringInterpVisualizer, SpringInterpVisualizer)