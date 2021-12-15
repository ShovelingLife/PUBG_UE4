// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class PUBG_UE4Target : TargetRules
{
	public PUBG_UE4Target( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "PUBG_UE4", "AI_PUBG", "UI_PUBG", "Animations", "Characters", "Farmable_items", "Player_weapons", "Server_PUBG" } );
	}
}
