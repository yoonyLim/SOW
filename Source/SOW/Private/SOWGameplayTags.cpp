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

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Restore_Health, "Player.Event.Restore.Health");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Restore_Mana, "Player.Event.Restore.Mana");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Restore_Stamina, "Player.Event.Restore.Stamina")
	
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Attack_Melee, "Enemy.Ability.Attack.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Attack_Ranged, "Enemy.Ability.Attack.Ranged");

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Initialize, "Enemy.Ability.Initialize");

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Buff_Stealth, "Enemy.Status.Buff.Stealth");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Buff_RangedResistance, "Enemy.Status.Buff.RangedResistance");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Buff_MeleeResistance, "Enemy.Status.Buff.MeleeResistance");

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Debuff_Taunted, "Enemy.Status.Debuff.Taunted");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Debuff_Slow, "Enemy.Status.Debuff.Slow");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Debuff_RestoreMovement, "Enemy.Status.Debuff.RestoreMovement");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Debuff_Rooted, "Enemy.Status.Debuff.Rooted");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Debuff_Detected, "Enemy.Status.Debuff.Detected"); 
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Debuff_Static, "Enemy.Status.Debuff.Static");

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Debuff_Chill, "Enemy.Status.Debuff.Chill");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Debuff_Freezing, "Enemy.Status.Debuff.Freezing");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Debuff_AntiMagic, "Enemy.Status.Debuff.AntiMagic");

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Debuff_Marking, "Enemy.Status.Debuff.Marking");

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Debuff_Sine_Alpha, "Enemy.Status.Debuff.Sine.Alpha");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Debuff_Sine_Beta, "Enemy.Status.Debuff.Sine.Beta");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Debuff_Sine_Gamma, "Enemy.Status.Debuff.Sine.Gamma");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Debuff_Sine_Delta, "Enemy.Status.Debuff.Sine.Delta");

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Debuff_Sine_Stun, "Enemy.Status.Debuff.Sine.Stun");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Debuff_Sine_Chellenger, "Enemy.Status.Debuff.Sine.Chellenger");

	
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Immunity_ShadowAdditionalDamage, "Enemy.Status.Immunity.ShadowAdditionalDamage");

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Immunity_Sine_Stun, "Enemy.Status.Immunity.Sine.Stun");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Immunity_Chill, "Enemy.Status.Immunity.Chill");

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Immunity_Sine_Alpha, "Enemy.Status.Immunity.Sine.Alpha");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Immunity_Sine_Beta, "Enemy.Status.Immunity.Sine.Beta");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Immunity_Sine_Gamma, "Enemy.Status.Immunity.Sine.Gamma");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Immunity_Sine_Delta, "Enemy.Status.Immunity.Sine.Delta");
	

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_Confusing, "Enemy.Event.Confusing"); 
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_Static, "Enemy.Event.Static");
	
	UE_DEFINE_GAMEPLAY_TAG(Turret_Ability_Attack, "Turret.Ability.Attack");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Ability_Attack_Melee, "Turret.Ability.Attack.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Ability_Attack_Ranged, "Turret.Ability.Attack.Ranged");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Ability_Buff, "Turret.Ability.Buff"); 
	UE_DEFINE_GAMEPLAY_TAG(Turret_Ability_Continue, "Turret.Ability.Continue");

	UE_DEFINE_GAMEPLAY_TAG(Turret_Ability_MeleeAttack, "Turret.Ability.MeleeAttack");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Ability_RangedAttack, "Turret.Ability.RangedAttack");

	UE_DEFINE_GAMEPLAY_TAG(Turret_Status_Buff_ForceMelee, "Turret.Status.Buff.ForceMelee");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Status_Buff_ForceRanged, "Turret.Status.Buff.ForceRanged");

	UE_DEFINE_GAMEPLAY_TAG(Turret_Status_Buff_Hoppy, "Turret.Status.Buff.Hoppy");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Status_Buff_BlessingOfRune, "Turret.Status.Buff.BlessingOfRune");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Status_Buff_SpiderWeb, "Turret.Status.Buff.SpiderWeb");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Status_Buff_NightEyes, "Turret.Status.Buff.NightEyes");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Status_Buff_Mushroom, "Turret.Status.Buff.Mushroom");

	UE_DEFINE_GAMEPLAY_TAG(Turret_Status_Buff_Snowball, "Turret.Status.Buff.Snowball");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Status_Buff_GlassyAura, "Turret.Status.Buff.GlassyAura");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Status_Buff_Detector, "Turret.Status.Buff.Detector");

	UE_DEFINE_GAMEPLAY_TAG(Turret_Status_Buff_Ember, "Turret.Status.Buff.Ember");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Status_Buff_Ignition, "Turret.Status.Buff.Ignition");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Status_Buff_Glare, "Turret.Status.Buff.Glare");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Status_Buff_Impatience, "Turret.Status.Buff.Impatience");

	UE_DEFINE_GAMEPLAY_TAG(Turret_Status_Debuff_Slow, "Turret.Status.Debuff.Slow");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Status_Debuff_Root, "Turret.Status.Debuff.Root");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Status_Debuff_Shadow, "Turret.Status.Debuff.Shadow");

	UE_DEFINE_GAMEPLAY_TAG(Turret_Status_Immunity_Buff, "Turret.Status.Immunity.Buff");
	
	UE_DEFINE_GAMEPLAY_TAG(Turret_Event_Attack_Basic, "Turret.Event.Attack.Basic"); ;
	UE_DEFINE_GAMEPLAY_TAG(Turret_Event_Attack_Done, "Turret.Event.Attack.Done");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Event_Attack_Double, "Turret.Event.Attack.Double"); 
	UE_DEFINE_GAMEPLAY_TAG(Turret_Event_EffectApplied, "Turret.Event.EffectApplied");

	UE_DEFINE_GAMEPLAY_TAG(Turret_Event_TikiSupporting, "Turret.Event.TikiSupporting");



	UE_DEFINE_GAMEPLAY_TAG(Turret_Evolution_Alpha, "Turret.Evolution.Alpha");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Evolution_Beta, "Turret.Evolution.Beta");
	
	UE_DEFINE_GAMEPLAY_TAG(Turret_Event_SetPropByEvo, "Turret.Event.SetPropByEvo"); 
	UE_DEFINE_GAMEPLAY_TAG(Turret_Event_TargetCountChanged, "Turret.Event.TargetCountChanged"); 
	UE_DEFINE_GAMEPLAY_TAG(Turret_Event_ChangedTurretLocation, "Turret.Event.ChangedTurretLocation");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Event_ChangedFixedLocation, "Turret.Event.ChangedFixedLocation");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Event_ChangedFixedTarget, "Turret.Event.ChangedFixedTarget"); 
	UE_DEFINE_GAMEPLAY_TAG(Turret_Event_DebuffApplied, "Turret.Event.DebuffApplied");

	UE_DEFINE_GAMEPLAY_TAG(Turret_Event_TargetCharacterOut, "Turret.Event.TargetCharacterOut");
	UE_DEFINE_GAMEPLAY_TAG(Turret_Event_TargetCharacterIn, "Turret.Event.TargetCharacterIn");

	UE_DEFINE_GAMEPLAY_TAG(Turret_Event_AffectStatUp, "Turret.Event.AffectStatUp");


	UE_DEFINE_GAMEPLAY_TAG(Rune_SetByCaller_HealBySynergy, "Rune.SetByCaller.HealBySynergy");


	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Death, "Shared.Ability.Death");

	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Dead, "Shared.Status.Dead"); 
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_DamagedOverTime, "Shared.Status.DamagedOverTime");


	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit, "Shared.Event.MeleeHit");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHitDone, "Shared.Event.MeleeHitDone");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact, "Shared.Event.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_TargetDead, "Shared.Event.TargetDead");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_TauntToTarget, "Shared.Event.TauntToTarget");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact_PercentDamage, "Shared.Event.HitReact.PercentDamage");
	

	
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

	UE_DEFINE_GAMEPLAY_TAG(Tile_Forest_Normal, "Tile.Forest.Normal")
		
	//UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_AdditiveAttackPower, "Shared.SetByCaller.AdditiveAttackPower");
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_AdditiveDamage, "Shared.SetByCaller.AdditiveDamage");
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_MultipleDamage, "Shared.SetByCaller.MultipleDamage");
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_AdditinalDamageRatio_Pulse, "Shared.SetByCaller.AdditinalDamageRatio.Pulse"); 
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_AdditionalDamageRatio, "Shared.SetByCaller.AdditionalDamageRatio");

	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_PercentageDamage, "Shared.SetByCaller.PercentageDamage");

	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_AdditiveBuffValue, "Shared.SetByCaller.AdditiveBuffValue");
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_ReductedDamage, "Shared.SetByCaller.ReductedDamage");
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_MushroomExtraDamageCont, "Shared.SetByCaller.MushroomExtraDamageCont");
	
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_RestoreMovement, "Shared.SetByCaller.RestoreMovement");

	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_ChillSlow, "Shared.SetByCaller.ChillSlow");
	

	UE_DEFINE_GAMEPLAY_TAG(World_Event_WaveStart, "World.Event.WaveStart");
	UE_DEFINE_GAMEPLAY_TAG(World_Event_Gen_Nature, "World.Event.Gen.Nature");
	UE_DEFINE_GAMEPLAY_TAG(World_Event_Gen_Electro, "World.Event.Gen.Electro");
	UE_DEFINE_GAMEPLAY_TAG(World_Event_Gen_Death, "World.Event.Gen.Death");
	UE_DEFINE_GAMEPLAY_TAG(World_Event_Gen_Wave, "World.Event.Gen.Wave");
	UE_DEFINE_GAMEPLAY_TAG(World_Event_Gen_Ice, "World.Event.Gen.Ice");
	UE_DEFINE_GAMEPLAY_TAG(World_Event_Gen_Divinity, "World.Event.Gen.Divinity");
	UE_DEFINE_GAMEPLAY_TAG(World_Event_Gen_Madness, "World.Event.Gen.Madness");
	UE_DEFINE_GAMEPLAY_TAG(World_Event_Gen_Flame, "World.Event.Gen.Flame");
	
	UE_DEFINE_GAMEPLAY_TAG(Data_ManaCost, "Data.ManaCost");
	UE_DEFINE_GAMEPLAY_TAG(Data_ModValue, "Data.ModValue");
	UE_DEFINE_GAMEPLAY_TAG(Data_RestoreMovement, "Data.RestoreMovement");
	
	/* Player Skill Tag */
	/* Nature Skills*/
	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_Nature_l, "Player.Skill.Nature.l");
	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_Nature_ll, "Player.Skill.Nature.ll");
	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_Nature_lll, "Player.Skill.Nature.lll");
	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_Nature_lV, "Player.Skill.Nature.lV");
	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_Nature_V, "Player.Skill.Nature.V");

	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_Richness_l, "Player.Skill.Richness.l");
	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_Richness_ll, "Player.Skill.Richness.ll");
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




namespace SOWSynergyTags {
	UE_DEFINE_GAMEPLAY_TAG(Synergy_Nature_III, "Synergy.Nature.III");
	UE_DEFINE_GAMEPLAY_TAG(Synergy_Nature_V, "Synergy.Nature.V");
	UE_DEFINE_GAMEPLAY_TAG(Synergy_Nature_VI, "Synergy.Nature.VI");
	UE_DEFINE_GAMEPLAY_TAG(Synergy_Nature_VII, "Synergy.Nature.VII");

	UE_DEFINE_GAMEPLAY_TAG(Synergy_Electro_I, "Synergy.Electro.I");
	UE_DEFINE_GAMEPLAY_TAG(Synergy_Electro_II, "Synergy.Electro.II");
	UE_DEFINE_GAMEPLAY_TAG(Synergy_Electro_III, "Synergy.Electro.III");
	UE_DEFINE_GAMEPLAY_TAG(Synergy_Electro_IV, "Synergy.Electro.IV");

	UE_DEFINE_GAMEPLAY_TAG(Synergy_Ice_II, "Synergy.Ice.II");
	UE_DEFINE_GAMEPLAY_TAG(Synergy_Ice_III, "Synergy.Ice.III");
	UE_DEFINE_GAMEPLAY_TAG(Synergy_Ice_IV, "Synergy.Ice.IV");
	UE_DEFINE_GAMEPLAY_TAG(Synergy_Ice_V, "Synergy.Ice.V");

	UE_DEFINE_GAMEPLAY_TAG(Synergy_Flame_II, "Synergy.Flame.II");
	UE_DEFINE_GAMEPLAY_TAG(Synergy_Flame_IV, "Synergy.Flame.IV");
	UE_DEFINE_GAMEPLAY_TAG(Synergy_Flame_V, "Synergy.Flame.V");
}

namespace SOWGlacioPropertyTags {
	UE_DEFINE_GAMEPLAY_TAG(Property_Alpha_I, "Property.Alpha.I");
	UE_DEFINE_GAMEPLAY_TAG(Property_Alpha_II, "Property.Alpha.II");
	UE_DEFINE_GAMEPLAY_TAG(Property_Alpha_III, "Property.Alpha.III");
	UE_DEFINE_GAMEPLAY_TAG(Property_Alpha_IV, "Property.Alpha.IV");
	UE_DEFINE_GAMEPLAY_TAG(Property_Alpha_V, "Property.Alpha.V");

	UE_DEFINE_GAMEPLAY_TAG(Property_Alpha, "Property.Alpha");

	UE_DEFINE_GAMEPLAY_TAG(Property_Beta_I, "Property.Beta.I");
	UE_DEFINE_GAMEPLAY_TAG(Property_Beta_II, "Property.Beta.II");
	UE_DEFINE_GAMEPLAY_TAG(Property_Beta_III, "Property.Beta.III");
	UE_DEFINE_GAMEPLAY_TAG(Property_Beta_IV, "Property.Beta.IV");
	UE_DEFINE_GAMEPLAY_TAG(Property_Beta_V, "Property.Beta.V");

	UE_DEFINE_GAMEPLAY_TAG(Property_Beta, "Property.Beta");
}