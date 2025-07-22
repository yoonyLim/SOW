// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SOW : ModuleRules
{
	public SOW(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"GamePlayAbilities",
			"GameplayTags",
			"GameplayTasks", 
			"AnimGraphRuntime", // Added by Yoony for Enemy AnimInstance
			"AIModule", // Added by Yoony for Enemy AI
			"NavigationSystem", // Added by Yoony for Enemy AI
			"UMG",
			"Slate", // Added by Hjy for UI
			"SlateCore", // Added by Hjy for UI
			"Niagara", // Added by Pgh for projectile effect
			"NiagaraShader" // Added by Pgh for projectile effect
        });
	}
}
