using UnrealBuildTool;

public class Objects : ModuleRules
{
    public Objects(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp17; 

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "Niagara" });
        PrivateDependencyModuleNames.AddRange(new string[] { "PUBG_UE4" });
    }
}