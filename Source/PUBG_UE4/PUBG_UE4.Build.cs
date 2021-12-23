// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PUBG_UE4 : ModuleRules
{
	public PUBG_UE4(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
		PrivateDependencyModuleNames.AddRange(new string[] { "UMG", "UI_PUBG" });
    }
}
