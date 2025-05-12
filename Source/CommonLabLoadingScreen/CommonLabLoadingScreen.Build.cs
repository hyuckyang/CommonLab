using UnrealBuildTool;

public class CommonLabLoadingScreen : ModuleRules
{
    public CommonLabLoadingScreen(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "UMG",
                "Slate",
                "SlateCore",
                "InputCore",
                "MoviePlayer",
                "DeveloperSettings",
            }
        );
    }
}