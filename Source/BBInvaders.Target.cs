// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BBInvadersTarget : TargetRules
{
	public BBInvadersTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        bUseAdaptiveUnityBuild = false;

		ExtraModuleNames.AddRange( new string[] { "BBInvaders" } );
	}
}
