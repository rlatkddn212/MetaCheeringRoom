// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MetaCheeringRoom : ModuleRules
{
	public MetaCheeringRoom(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Json", "JsonUtilities", "HTTP", "MediaAssets", "OnlineSubsystem", "OnlineSubsystemSteam", "ProceduralMeshComponent","MeshDescription", "StaticMeshDescription" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        PrivateIncludePaths.Add("MetaCheeringRoom");
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
