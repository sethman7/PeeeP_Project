// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PeeeP_Prototype : ModuleRules
{
	public PeeeP_Prototype(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { "PeeeP_Prototype" });

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"Niagara", 
			"UMG", 
			"MediaAssets",
            "MovieScene",
            "LevelSequence",
			"PhysicsCore"
		});

		//PrivateDependencyModuleNames.AddRange(new string[] {  });

		//PrivateIncludePaths.Add("PeeeP_Prototype");

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
