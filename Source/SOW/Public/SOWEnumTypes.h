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
	HighHealth,
	LowHealth,
	HighAttack,
	Nearest,
	Farthest,
	Custom
};

UENUM(BlueprintType)
enum class EStat : uint8 {
	HP,
	MP,
	Stamina
};