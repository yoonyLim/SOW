// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ESOWCharacterType : uint8 {
	Uncertain,
	Player,
	Turret,
	Enemy
};

UENUM(BlueprintType)
enum class ETurretTargetSelectionPolicy : uint8 {
	Uncertain,
	OnPlayer,
	OnTurret,
	OnEnemy
};

UENUM(BlueprintType)
enum class ETurretRarity : uint8 {
	Common,
	Uncommon,
	Rare,
	Epic
};


UENUM(BlueprintType)
enum class ETurretTargetSelectionType : uint8 {
	Single,
	Double,
	All,
};

UENUM(BlueprintType)
enum class ETurretTargetSelectionPriority : uint8 {
	HighHealth,
	LowHealth,
	HighAttack,
	Nearest,
	Farthest,
	Custom
UENUM(BlueprintType)
enum class EStat : uint8 {
	HP,
	MP,
	Stamina
};