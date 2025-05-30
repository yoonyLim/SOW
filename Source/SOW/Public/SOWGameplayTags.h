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
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Debug1);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Summon);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Rolling);

	/* Enemy Tags */
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Attack_Melee);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Attack_Ranged);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Initialize);

	/* Turret Tags */
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Ability_Attack);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Ability_Attack_Melee);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Ability_Attack_Ranged);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Ability_Buff);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Status_Buff_Hoppy);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Status_Debuff_Slow);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Status_Debuff_Root);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Event_Attack_Basic);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Evolution_Alpha);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Evolution_Beta);

	/* Shared Tags */
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Ability_Death);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Dead);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_DamagedOverTime);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_HitReact);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_TargetDead);


	/* Element Tags */
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Nature_Major);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Nature_Minor);
	
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Electro_Major);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Electro_Minor);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Death_Major);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Death_Minor);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Ice_Major);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Ice_Minor);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Wave_Major);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Wave_Minor);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Divinity_Major);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Divinity_Minor);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Madness_Major);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Madness_Minor);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Flame_Major);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Element_Flame_Minor);
}
