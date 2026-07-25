// Copyright (c) Mistfly Games. All Rights Reserved.

using UnrealBuildTool;

public class SeedworkRandom : ModuleRules
{
    public SeedworkRandom(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
        [
            "Core"
        ]);

        PrivateDependencyModuleNames.AddRange(
        [
            "CoreUObject",
            "Engine",
        ]);
    }
}
