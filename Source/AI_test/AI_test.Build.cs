// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AI_test : ModuleRules
{
	public AI_test(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "NavigationSystem", "GameplayTasks" });
	}
}
