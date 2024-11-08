// Copyright 2021 ls Sun, Inc. All Rights Reserved.

#include "RuntimeLoadFbx.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FRuntimeLoadFbxModule"

void FRuntimeLoadFbxModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("RuntimeLoadFbx")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	//FString LibraryPath, LibraryPath2;
    TArray<FString> dllPaths;
#if PLATFORM_WINDOWS
    //dllPaths.Add(FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/Win64/FbxImExPort.dll")));
    dllPaths.Add(FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/FbxImExPort/x64/Release/FbxImExPort.dll")));
#elif PLATFORM_MAC
    dllPaths.Add(FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/FbxImExPort/Mac/Release/FbxImExPort.dylib")));
#endif // PLATFORM_WINDOWS

    for (int i=0;i<dllPaths.Num();++i)
    {
        const FString &LibraryPath = dllPaths[i];
        void* ExampleLibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;
        if (!ExampleLibraryHandle)
        {
            FString strInfo = FString::Printf(TEXT("load %s failed..."), *LibraryPath);
            FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(strInfo));
        }
        else
        {
            libs.Add(ExampleLibraryHandle);
        }
    }

}

void FRuntimeLoadFbxModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	// Free the dll handle
    for (int i = 0; i < libs.Num(); ++i)
    {
        FPlatformProcess::FreeDllHandle(libs[i]);
    }

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRuntimeLoadFbxModule, RuntimeLoadFbx)
