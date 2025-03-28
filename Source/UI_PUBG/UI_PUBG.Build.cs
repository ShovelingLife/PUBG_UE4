using UnrealBuildTool;
 
public class UI_PUBG : ModuleRules
{
    public UI_PUBG(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp20;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "UMG", "InputCore" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "PUBG_UE4", "Characters", "Player_weapons", "Farmable_items" });
    }
}