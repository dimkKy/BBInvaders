// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BBInvadersEditorTarget : TargetRules
{
	public BBInvadersEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        bUseAdaptiveUnityBuild = false;

        ExtraModuleNames.AddRange( new string[] { "BBInvaders" } );
	}
}
