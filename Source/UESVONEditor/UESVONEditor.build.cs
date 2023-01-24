using UnrealBuildTool;

namespace UnrealBuildTool.Rules
{
    public class UESVONEditor : ModuleRules
    {
        public UESVONEditor(TargetInfo Target)
        {

            PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine",  "UESVON", "InputCore"});

            PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "PropertyEditor", "EditorStyle", "UnrealEd", "GraphEditor", "BlueprintGraph" });
            
            PrivateIncludePaths.AddRange(new string[] { "UESVONEditor/Private"	} );

            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        }
    }
}