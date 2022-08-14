using UnrealBuildTool;
 
public class Player_weapons : ModuleRules
{
    public Player_weapons(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp20;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "PUBG_UE4", "Farmable_items", "Objects" });
        PrivateDependencyModuleNames.AddRange(new string[] { "InputCore" });
    }
}