// Fill out your copyright notice in the Description page of Project Settings.
 
using UnrealBuildTool;
 
public class Farmable_items : ModuleRules
{
    public Farmable_items(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
 
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
        PrivateDependencyModuleNames.AddRange(new string[] { "PUBG_UE4" });
    }
}