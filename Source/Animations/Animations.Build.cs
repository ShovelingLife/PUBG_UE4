// Fill out your copyright notice in the Description page of Project Settings.
 
using UnrealBuildTool;
 
public class Animations : ModuleRules
{
    public Animations(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp20;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
        PrivateDependencyModuleNames.AddRange(new string[] { "AnimGraphRuntime", "Characters", "AI_PUBG", "PUBG_UE4", "Player_weapons" });
    }
}