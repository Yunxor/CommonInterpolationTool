using UnrealBuildTool;

public class SpringInterpVisualizer : ModuleRules
{
    public SpringInterpVisualizer(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
                "Engine",
                "EditorStyle",
                "UnrealEd",
                "Blutility",
                "WorkspaceMenuStructure"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Slate",
                "SlateCore",
                "UMG",
                "UMGEditor",
                "InputCore",
                "CommonInterpolationVisualizer"
            }
        );
    }
}