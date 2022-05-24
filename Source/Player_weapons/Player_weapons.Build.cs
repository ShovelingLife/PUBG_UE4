using UnrealBuildTool;
 
public class Player_weapons : ModuleRules
{
    public Player_weapons(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
 
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "PUBG_UE4" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Niagara", "Farmable_items" });
    }
}