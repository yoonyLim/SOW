// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ESOWCharacterType : uint8 {
	// added by : PGH
	// Check What Character Type is or the Actor is SOWCharacter
	Uncertain,
	CoreRune,
	Player,
	Turret,
	Enemy
};

UENUM(BlueprintType)
enum class ETurretTargetSelectionPolicy : uint8 {
	// added by : PGH
	// Check What Turret's target is 
	Uncertain,
	CoreRune,
	OnPlayer,
	OnTurret,
	OnEnemy
};

UENUM(BlueprintType)
enum class ETurretRarity : uint8 {
	// added by : PGH
	// Check What is turret's Rarity 
	Common,
	Uncommon,
	Rare,
	Epic
};


UENUM(BlueprintType)
enum class ETurretTargetSelectionType : uint8 {
	// added by : PGH
	// Check How many target Turret has 
	Single,
	Double,
	All,
};

UENUM(BlueprintType)
enum class ETurretTargetSelectionPriority : uint8 {
	// added by : PGH
	// Check how the turret determines the target
	Uncertain,
	HighHealth,
	LowHealth,
	HighAttack,
	Nearest,
	Farthest,
	LocationFixed,
	TargetFixed
};

UENUM(BlueprintType)
enum class ETurretName : uint8 {
	// added by : PGH
	// Nature
	Bowman			UMETA(DisplayName = "Bowman"),
	Hoppy			UMETA(DisplayName = "Hoppy"),
	Evilwood		UMETA(DisplayName = "Evilwood"),
	Flower			UMETA(DisplayName = "Flower"),
	Golem			UMETA(DisplayName = "Golem"),
	Mushroom		UMETA(DisplayName = "Mushroom"),
	Owl				UMETA(DisplayName = "Owl"),
	Spider			UMETA(DisplayName = "Spider"),

	// Electro
	DesertBat		UMETA(DisplayName = "DesertBat"),
	DesertWizard	UMETA(DisplayName = "DesertWizard"),
	DesertCactus	UMETA(DisplayName = "DesertCactus"),
	DesertGolem		UMETA(DisplayName = "DesertGolem"),

	// Ice
	IceCitizen		UMETA(DisplayName = "IceCitizen"),
	IceGolem		UMETA(DisplayName = "IceGolem"),
	Yetti			UMETA(DisplayName = "Yetti"),
	IceBreaker		UMETA(DisplayName = "IceBreaker"),
	Wildboar		UMETA(DisplayName = "Wildboar"),

	// Flame
	Dragon			UMETA(DisplayName = "Dragon"),
	Tiki			UMETA(DisplayName = "Tiki"),
	TikiGhost		UMETA(DisplayName = "TikiGhost"),
	VolcanoGolem	UMETA(DisplayName = "VolcanoGolem"),
	VolcanoLizard	UMETA(DisplayName = "VolcanoLizard"),

	// Special Unit
	Glacio			UMETA(DisplayName = "Glacio")
	
};

UENUM(BlueprintType)
enum class EEvolutionType : uint8 {
	// added by : PGH
	// determine turret evolution type 
	EVO_ALPHA	UMETA(DisplayName = "Alpha"),
	EVO_BETA	UMETA(DisplayName = "Beta"),
	EVO_PROP	UMETA(DisplayName = "Property")
};


UENUM(BlueprintType)
enum class EStat : uint8 {
	HP,
	MP,
	Stamina
};

UENUM(BlueprintType)
enum class EMagicSpell : uint8 {
	Vadh,
	Drm,
	Bhus,
	Rudh,
	Badh
};

UENUM(BlueprintType)
enum class ETargetFixErrorType : uint8 {
	SUCCESS,
	OUT_OF_RANGE,
	INVALID_TARGET,
	TURRET_DEAD
};

UENUM(BlueprintType)
enum class EWidgetDescriptedAtt : uint8 {
	MAXHEALTH,
	ATTACKPOWER,
	DEFENSEPOWER,
	ATTACKSPEED
};

UENUM(BlueprintType)
enum class ESkillExecuteType : uint8 {
	// added by : HYJ
	// Check how the skill acts
	Active				UMETA(DisplayName = "Circle Upgrade"),
	Passive			UMETA(DisplayName = "Attribute Modifier"),
};

UENUM(BlueprintType)
enum class ESkillEffectType : uint8 {
	// added by : HYJ
	// Check how the skill acts
	CircleUpgrade				UMETA(DisplayName = "Circle Upgrade"),
	AttributeModifier			UMETA(DisplayName = "Attribute Modifier"),
	CustomScript				UMETA(DisplayName = "CustomScript"),
};

UENUM(BlueprintType)
enum class EPassiveSkillTrigger : uint8
{
	// addedby : HJY
	// Check the skill activation conditions
	None,
	OnTurretSpawn,
	OnWaveStart,
	InOverlappingInfluenceZone,

	/* Nature Turret */
	InForestArea,
};

UENUM(BlueprintType)
enum class ESkillTargetType : uint8
{
	// addedby : HJY
	// Check the type of skill applied to
	None,
	Self,
	Enemy,
	Ally,
	EnemyInRange,
	AllyInRange,
};

UENUM(BlueprintType)
enum class EElementalType : uint8
{
	// added by : HYJ
	Normal,
	Nature,
	Electro,
	Death,
	Ice,
	Wave,
	Divinity,
	Madness,
	Flame,
	Max		UMETA(Hidden),
};

UENUM(BlueprintType)
enum class ETileSelectType : uint8 {
	// added by : PGH
	// check tile selection mode
	SQUARED		UMETA(DisplayName = "Squared"),
	STRAIGHT	UMETA(DisplayName = "Straight")
};


UENUM(BlueprintType)
enum class ETileSummonState : uint8
{
	// added by : HJY
	Available,
	Unavailable,
	Occupied,
};

UENUM(BlueprintType)
enum class EGlacioStatType : uint8
{
	AttackPower,
	AttackSpeed,
	DetectionRange,
	DebuffCount,
	ExtraDamage,
	None
};