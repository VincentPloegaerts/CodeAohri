// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class InsideUs : ModuleRules
{
	public InsideUs(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore" , "Niagara", "UMG", "SlateCore", "Slate", "AIModule", "GameplayTasks",
			"OnlineSubsystem", "OnlineSubsystemUtils", "OnlineSubsystemSteam", "ApplicationCore", "JsonUtilities", "Json", "CinematicCamera",
			"LevelSequence", "MovieScene", "MediaAssets"
		});
		
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Steam", "lib", "steam_api64.lib"));
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"OnlineSubsystem", "OnlineSubsystemNull", "OnlineSubsystemSteam",  "FMODStudio"
        });

        if (Target.bBuildEditor)
		{
			PublicDependencyModuleNames.AddRange(new string[] { "EditorWidgetCreator", "Blutility", "UMGEditor" });
		}

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
