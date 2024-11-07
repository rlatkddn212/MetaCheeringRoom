// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class FbxImExPort : ModuleRules
{
	public FbxImExPort(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Add the import library
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "FbxImExPort.lib"));

			// Delay-load the DLL, so we can load it from the right place first
			PublicDelayLoadDLLs.Add("FbxImExPort.dll");

			// Ensure that the DLL is staged along with the executable
			//RuntimeDependencies.Add("$(PluginDir)/Binaries/ThirdParty/Win64/FbxImExPort.dll");
			RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "x64", "Release", "FbxImExPort.dll"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "FbxImExPort.dylib"));
            //RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/Mac/Release/libExampleLibrary.dylib");
            RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "FbxImExPort.dylib"));
        }
    }
}
