// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

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