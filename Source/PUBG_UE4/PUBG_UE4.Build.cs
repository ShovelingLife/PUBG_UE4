// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PUBG_UE4 : ModuleRules
{
	public PUBG_UE4(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp20;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" });
		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
