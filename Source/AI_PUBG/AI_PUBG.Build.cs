// Fill out your copyright notice in the Description page of Project Settings.
 
using UnrealBuildTool;
 
public class AI_PUBG : ModuleRules
{
    public AI_PUBG(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
 
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "AIModule", "NavigationSystem", "GameplayTasks" });
 
        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}