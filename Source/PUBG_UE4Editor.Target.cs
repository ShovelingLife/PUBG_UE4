// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class PUBG_UE4EditorTarget : TargetRules
{
	public PUBG_UE4EditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "PUBG_UE4", "AI_PUBG", "UI_PUBG", "Animations", "Characters", "Farmable_items", "Player_weapons" } );
	}
}
