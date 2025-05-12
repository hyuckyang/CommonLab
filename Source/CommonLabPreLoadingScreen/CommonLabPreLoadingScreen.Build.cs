using UnrealBuildTool;

public class CommonLabPreLoadingScreen : ModuleRules
{
    public CommonLabPreLoadingScreen(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", "Slate"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "PreLoadScreen",
                "DeveloperSettings",
            }
        );
    }
}