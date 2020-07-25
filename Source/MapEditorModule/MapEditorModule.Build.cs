// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MapEditorModule : ModuleRules
{
    public MapEditorModule(ReadOnlyTargetRules Target) : base(Target)
    {

        bTreatAsEngineModule = true;

        PublicDependencyModuleNames.AddRange(new string[]{
            "Core",
            "CoreUObject",
            "InputCore",
            "Engine",
            "MapGenerator",
            "UnrealEd",
            "SlateCore",
            "Slate",
            "EditorStyle"
        });

        PrivateDependencyModuleNames.AddRange(new string[]{
        });

        PrivateDependencyModuleNames.AddRange(new string[]{
        });

        PrivateIncludePathModuleNames.AddRange(new string[]{
        });

        DynamicallyLoadedModuleNames.AddRange(new string[]{
        });

    }
}
