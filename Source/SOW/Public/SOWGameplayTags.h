// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace SOWGameplayTags {
	/* Input Tags */
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Roll);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UseAbility);

	/* Player Tags */
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Debug);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Roll);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Rolling);

	/* Enemy Tags */
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Attack_Melee);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Attack_Ranged);

	/* Turret Tags */
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Ability_Attack_Melee);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Ability_Attack_Ranged);

	/* Shared Tags */
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Ability_Death);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Dead);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_HitReact);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Major_Nature);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Major_Flame);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Major_Death);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Major_Ice);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Major_Wave);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Major_Divinity);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Major_Madness);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Minor_Nature);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Minor_Flame);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Minor_Death);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Minor_Ice);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Minor_Wave);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Minor_Divinity);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Minor_Madness);
}
