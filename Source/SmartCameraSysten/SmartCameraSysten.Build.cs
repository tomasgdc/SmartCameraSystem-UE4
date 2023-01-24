// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;

namespace UnrealBuildTool.Rules
{
	public class SmartCameraSysten : ModuleRules
	{
		public SmartCameraSysten(TargetInfo Target)
		{
			PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
			
			PublicIncludePaths.AddRange(
				new string[] {
					// ... add public include paths required here ...
				}
				);
					
			
			PrivateIncludePaths.AddRange(
				new string[] {
					// ... add other private include paths required here ...
				}
				);
				
			
			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"UMG"
					// ... add other public dependencies that you statically link with here ...
				}
				);
				
			
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"CoreUObject",
					"Engine",
					"Slate",
					"SlateCore",
					"Renderer",
					"RHI",
					"RenderCore",
					"UnrealEd"
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
}