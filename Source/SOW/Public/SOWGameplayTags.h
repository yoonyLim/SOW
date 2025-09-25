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
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Fail_Ability);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Restore_Health);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Restore_Mana);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Restore_Stamina);

	/* Enemy Tags */
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Attack_Melee);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Attack_Ranged);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Initialize);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Debuff_Taunted);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Debuff_Poison);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Debuff_SpiderWeb);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Debuff_Slow);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Debuff_Rooted);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Debuff_RestoreMovement);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Debuff_Detected);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Debuff_Static);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Debuff_Sine_Alpha);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Debuff_Sine_Stun);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Debuff_Sine_Beta);
	//SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Debuff_Static);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Debuff_Sine_Gamma);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Debuff_Sine_Chellenger);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Debuff_Sine_Delta);
	//SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Debuff_Balancing);


	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Immunity_ShadowAdditionalDamage);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Immunity_Sine_Stun);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Immunity_Sine_Alpha);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Immunity_Sine_Beta);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Immunity_Sine_Gamma);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Immunity_Sine_Delta);
	//SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Immune_Balancing);


	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Event_Confusing);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Event_Static);
	
	
	

	/* Turret Tags */
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Ability_Attack);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Ability_Attack_Melee);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Ability_Attack_Ranged);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Ability_Buff);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Status_Buff_Hoppy);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Status_Buff_BlessingOfRune);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Status_Buff_SpiderWeb);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Status_Buff_NightEyes); 
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Status_Buff_Mushroom);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Status_Debuff_Slow);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Status_Debuff_Root);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Status_Debuff_Shadow);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Status_Immunity_Buff);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Event_Attack_Basic);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Event_Attack_Done);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Event_Attack_Double);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Event_EffectApplied);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Evolution_Alpha);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Evolution_Beta);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Event_SetPropByEvo);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Event_TargetCountChanged);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Event_ChangedFixedLocation);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Event_ChangedFixedTarget);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Event_DebuffApplied);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Event_TargetCharacterOut);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Turret_Event_TargetCharacterIn);

	/* Shared Tags */
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Ability_Death);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Dead);					// 캐릭터의 현재 체력이 0일때 부여되는 상태 태그
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_DamagedOverTime);		// 해당 상태 태그가 부여된 대상은 DamageOverTime 값만큼 지속 피해를 입음.

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_MeleeHit);				// 캐릭터가 공격에 성공했을 경우 부여하는 이벤트 태그
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_HitReact);				// 캐릭터에 공격에 맞을 경우 피격자에게 부여되는 이벤트 태그
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_TargetDead);			// 공격한 타겟이 사망했을 경우 공격자에게 전달되는 이벤트 태그
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_TauntToTarget);			// 도발 성공 시 타겟에게 전달되는 이벤트 태그
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_MeleeHitDone);


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
	
	/* Tile Tags */
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tile_Forest_Normal);

	/* World Tags */
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(World_Event_WaveStart);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(World_Event_Gen_Nature);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(World_Event_Gen_Electro);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(World_Event_Gen_Death);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(World_Event_Gen_Ice);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(World_Event_Gen_Wave);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(World_Event_Gen_Divinity);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(World_Event_Gen_Madness);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(World_Event_Gen_Flame);

	/* Set By Caller Tags */
	//SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_AdditiveAttackPower);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_AdditiveDamage);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_MultipleDamage);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_AdditinalDamageRatio_Pulse);


	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_PercentageDamage);
	

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_AdditiveBuffValue);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_ReductedDamage);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_MushroomExtraDamageCont);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_RestoreMovement);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_RestoreMovement);

	/* Data Tags */
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_ManaCost);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_ModValue);
	
	/* Player Skill Tags */
	/* Nature Skills */
	/* Data Tags */
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Skill_Nature_l);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Skill_Nature_ll);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Skill_Nature_lll);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Skill_Nature_lV);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Skill_Nature_V);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Skill_Richness_l);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Skill_Richness_ll);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Skill_Richness_lll);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Skill_Richness_lV);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Skill_Richness_V);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Skill_CycleOfLife_l);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Skill_CycleOfLife_ll);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Skill_CycleOfLife_lll);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Skill_CycleOfLife_lV);

	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Skill_Creation_l);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Skill_Creation_ll);
}


namespace SOWSynergyTags {
	// Need to Implement Turret Synergy System only
	// When a turret is summoned, the synergy that matches the turret's Element reacts to grant a special effect to all turrets of the same Element that the instance is monitoring.

	// Nature Synergy
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Synergy_Nature_III);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Synergy_Nature_V);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Synergy_Nature_VI);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Synergy_Nature_VII);

	// Electro Synergy
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Synergy_Electro_I);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Synergy_Electro_II);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Synergy_Electro_III);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Synergy_Electro_IV);

	// Ice Synergy
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Synergy_Ice_II);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Synergy_Ice_III);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Synergy_Ice_IV);
	SOW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Synergy_Ice_V);
}
