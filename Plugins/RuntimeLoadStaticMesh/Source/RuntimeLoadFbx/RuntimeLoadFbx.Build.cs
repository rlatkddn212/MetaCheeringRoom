// Copyright 2021 ls Sun, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class RuntimeLoadFbx : ModuleRules
{
	public RuntimeLoadFbx(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		string fbx3rd = Path.Combine(ModuleDirectory, "../ThirdParty");

        PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
				fbx3rd
            }
            );
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
				fbx3rd
            }
            );
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "CoreUObject",
                "Engine",
                "Projects",                
                "RHI",
				"FbxImExPort",
				"ProceduralMeshComponent",
                "MeshDescription",
                "StaticMeshDescription",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...	
                
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
