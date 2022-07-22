using UnrealBuildTool;
 
public class Characters : ModuleRules
{
    public Characters(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp20;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "ChaosVehicles", "PUBG_UE4", "AI_PUBG", "Player_weapons", "Farmable_items", "InstaDeformPlugin" });
    }
}