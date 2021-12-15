// Fill out your copyright notice in the Description page of Project Settings.
 
using UnrealBuildTool;
 
public class Server_PUBG : ModuleRules
{
    public Server_PUBG(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
 
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
 
        PrivateDependencyModuleNames.AddRange(new string[] { "UI_PUBG", "UMG" });
    }
}