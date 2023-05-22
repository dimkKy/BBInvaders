// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BBInvaders : ModuleRules
{
	public BBInvaders(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp20;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore"/*, "UnrealEd"*/ });
		
	}
}
