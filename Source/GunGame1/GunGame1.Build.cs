// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GunGame1 : ModuleRules
{
	public GunGame1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", 
															"InputCore" ,"Slate", "SlateCore", 
															"UMG", "PhysicsCore","AIModule",
															"NavigationSystem"});
	}
}
