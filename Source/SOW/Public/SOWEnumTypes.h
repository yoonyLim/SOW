// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ESOWCharacterType : uint8 {
	// added by : PGH
	// Check What Character Type is or the Actor is SOWCharacter
	Uncertain,
	Player,
	Turret,
	Enemy
};

UENUM(BlueprintType)
enum class ETurretTargetSelectionPolicy : uint8 {
	// added by : PGH
	// Check What Turret's target is 
	Uncertain,
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
	DesertGolem		UMETA(DisplayName = "DesertGolem")
	
};

UENUM(BlueprintType)
enum class EEvolutionType : uint8 {
	EVO_NONE	UMETA(DisplayName = "None"),
	EVO_ALPHA	UMETA(DisplayName = "Alpha"),
	EVO_BETA	UMETA(DisplayName = "Beta")
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