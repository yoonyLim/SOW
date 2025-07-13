#include "SOWGameplayTags.h"
namespace SOWGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Roll, "InputTag.Roll"); 
	UE_DEFINE_GAMEPLAY_TAG(InputTag_UseAbility, "InputTag.UseAbility");

	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Debug, "Player.Ability.Attack.Debug");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Roll, "Player.Ability.Roll"); 
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Debug1, "Player.Ability.Debug1");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Summon, "Player.Ability.Summon");

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Rolling, "Player.Status.Rolling");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Fail_Ability, "Player.Status.Fail.Ability");
	
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Attack_Melee, "Enemy.Ability.Attack.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Attack_Ranged, "Enemy.Ability.Attack.Ranged");

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Initialize, "Enemy.Ability.Initialize");

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Taunted, "Enemy.Status.Taunted");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Poison, "Enemy.Status.Poison");
	
	UE_DEFINE_GAMEPLAY_TAG(Turret_Ability_Attack, "Turret.Ability.Attack");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Ability_Attack_Melee, "Turret.Ability.Attack.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Ability_Attack_Ranged, "Turret.Ability.Attack.Ranged");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Ability_Buff, "Turret.Ability.Buff"); 
	
	UE_DEFINE_GAMEPLAY_TAG(Turret_Status_Buff_Hoppy, "Turret.Status.Buff.Hoppy");

	UE_DEFINE_GAMEPLAY_TAG(Turret_Status_Debuff_Slow, "Turret.Status.Debuff.Slow");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Status_Debuff_Root, "Turret.Status.Debuff.Root");
	
	UE_DEFINE_GAMEPLAY_TAG(Turret_Event_Attack_Basic, "Turret.Event.Attack.Basic");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Event_Attack_Double, "Turret.Event.Attack.Double");
	
	UE_DEFINE_GAMEPLAY_TAG(Turret_Evolution_Alpha, "Turret.Evolution.Alpha");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Evolution_Beta, "Turret.Evolution.Beta");
	
	UE_DEFINE_GAMEPLAY_TAG(Turret_Event_SetPropByEvo, "Turret.Event.SetPropByEvo"); 
	UE_DEFINE_GAMEPLAY_TAG(Turret_Event_TargetCountChanged, "Turret.Event.TargetCountChanged"); 
	UE_DEFINE_GAMEPLAY_TAG(Turret_Event_ChangedFixedLocation, "Turret.Event.ChangedFixedLocation");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Event_ChangedFixedTarget, "Turret.Event.ChangedFixedTarget");

	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Death, "Shared.Ability.Death");

	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Dead, "Shared.Status.Dead"); 
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_DamagedOverTime, "Shared.Status.DamagedOverTime");


	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit, "Shared.Event.MeleeHit");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHitDone, "Shared.Event.MeleeHitDone");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact, "Shared.Event.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_TargetDead, "Shared.Event.TargetDead");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_TauntToTarget, "Shared.Event.TauntToTarget");
	

	
	UE_DEFINE_GAMEPLAY_TAG(Shared_Element_Nature_Major, "Shared.Element.Nature.Major");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Element_Nature_Minor, "Shared.Element.Nature.Minor");

	UE_DEFINE_GAMEPLAY_TAG(Shared_Element_Electro_Major, "Shared.Element.Electro.Major");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Element_Electro_Minor, "Shared.Element.Electro.Minor");

	UE_DEFINE_GAMEPLAY_TAG(Shared_Element_Death_Major, "Shared.Element.Death.Major");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Element_Death_Minor, "Shared.Element.Death.Minor");

	UE_DEFINE_GAMEPLAY_TAG(Shared_Element_Ice_Major, "Shared.Element.Ice.Major");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Element_Ice_Minor, "Shared.Element.Ice.Minor");

	UE_DEFINE_GAMEPLAY_TAG(Shared_Element_Wave_Major, "Shared.Element.Wave.Major");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Element_Wave_Minor, "Shared.Element.Wave.Minor");

	UE_DEFINE_GAMEPLAY_TAG(Shared_Element_Divinity_Major, "Shared.Element.Divinity.Major");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Element_Divinity_Minor, "Shared.Element.Divinity.Minor");

	UE_DEFINE_GAMEPLAY_TAG(Shared_Element_Madness_Major, "Shared.Element.Madness.Major");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Element_Madness_Minor, "Shared.Element.Madness.Minor");

	UE_DEFINE_GAMEPLAY_TAG(Shared_Element_Flame_Major, "Shared.Element.Flame.Major");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Element_Flame_Minor, "Shared.Element.Flame.Minor");

	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_AdditiveDamage, "Shared.SetByCaller.AdditiveDamage");
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_MultipleDamage, "Shared.SetByCaller.MultipleDamage");
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_AdditiveBuffValue, "Shared.SetByCaller.AdditiveBuffValue");
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_ReductedDamage, "Shared.SetByCaller.ReductedDamage");

	UE_DEFINE_GAMEPLAY_TAG(Data_ManaCost, "Data.ManaCost");

	/* Player Skill Tag */
	/* Nature Skills*/
	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_Nature_l, "Player.Skill.Nature.l");
	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_Nature_ll, "Player.Skill.Nature.ll");
	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_Nature_lll, "Player.Skill.Nature.lll");
	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_Nature_lV, "Player.Skill.Nature.lV");
	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_Nature_V, "Player.Skill.Nature.V");

	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_Richness_l, "Player.Skill.Richness.l");
	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_Richness_ll , "Player.Skill.Richness.ll");
	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_Richness_lll, "Player.Skill.Richness.lll");
	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_Richness_lV, "Player.Skill.Richness.lV");
	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_Richness_V, "Player.Skill.Richness.V");

	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_CycleOfLife_l, "Player.Skill.CycleOfLife.l");
	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_CycleOfLife_ll, "Player.Skill.CycleOfLife.ll");
	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_CycleOfLife_lll, "Player.Skill.CycleOfLife.lll");
	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_CycleOfLife_lV, "Player.Skill.CycleOfLife.lV");

	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_Creation_l, "Player.Skill.Creation.l");
	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_Creation_ll, "Player.Skill.Creation.ll");

}