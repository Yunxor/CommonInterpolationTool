#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSpringInterpVisualizer : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    void AddMenuEntry(FMenuBuilder& MenuBuilder);
    void OpenCustomEditorWindow();


    TSharedPtr<FExtender> MenuExtender;
    TWeakPtr<SWindow> CustomWindowInstance;
};
