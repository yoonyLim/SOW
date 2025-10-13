// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyEnums.generated.h"

UENUM(BlueprintType)
enum class EEnemyStates : uint8
{
	None UMETA(DisplayName = "None"),
	Passive UMETA(DisplayName = "Passive"),
	Attacking UMETA(DisplayName = "Attacking"),
	FollowingIncomingRoute UMETA(DisplayName = "FollowingIncomingRoute"),
	Taunted UMETA(DisplayName = "Taunted"),
	Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EEnemyTypes : uint8
{
	// Desert
	DesertBat UMETA(DisplayName = "DesertBat"),
	DesertMage UMETA(DisplayName = "DesertMage"),
	PunchCactus UMETA(DisplayName = "PunchCactus"),
	PyramidGolem UMETA(DisplayName = "PyramidGolem"),
	RockGolem UMETA(DisplayName = "RockGolem"),

	// Forest
	Bowman UMETA(DisplayName = "Bowman"),
	EvilFlower UMETA(DisplayName = "EvilFlower"),
	EvilMushroom UMETA(DisplayName = "EvilMushroom"),
	EvilWood UMETA(DisplayName = "EvilWood"),
	Golem UMETA(DisplayName = "Golem"),
	Hoppy UMETA(DisplayName = "Hoppy"),
	Owl UMETA(DisplayName = "Owl"),
	Spider UMETA(DisplayName = "Spider"),
	Taurus UMETA(DisplayName = "Taurus"),

	// Horror
	Bat UMETA(DisplayName = "Bat"),
	Doll UMETA(DisplayName = "Doll"),
	Ghost UMETA(DisplayName = "Ghost"),
	HorrorGolem UMETA(DisplayName = "HorrorGolem"),
	PaintingMonster UMETA(DisplayName = "PaintingMonster"),
	RoundKnight UMETA(DisplayName = "RoundKnight"),

	// Iceland
	BabyYeti UMETA(DisplayName = "BabyYeti"),
	IceCitizen UMETA(DisplayName = "IceCitizen"),
	IceGolem UMETA(DisplayName = "IceGolem"),
	Mammoth UMETA(DisplayName = "Mammoth"),
	WildBoar UMETA(DisplayName = "WildBoar"),
	Yetti UMETA(DisplayName = "Yetti"),

	// Ocean
	BlowFish UMETA(DisplayName = "BlowFish"),
	BombShark UMETA(DisplayName = "BombShark"),
	Clione UMETA(DisplayName = "Clione"),
	OceanGolem UMETA(DisplayName = "OceanGolem"),
	Octopus UMETA(DisplayName = "Octopus"),
	SeaUrchin UMETA(DisplayName = "SeaUrchin"),
	TreasureTurtle UMETA(DisplayName = "TreasureTurtle"),

	// Toyland
	BalloonGolem UMETA(DisplayName = "BalloonGolem"),
	BoxDoll UMETA(DisplayName = "BoxDoll"),
	IronBeak UMETA(DisplayName = "IronBeak"),

	// Volcano
	Dragon UMETA(DisplayName = "Dragon"),
	Tiki UMETA(DisplayName = "Tiki"),
	TikiGhost UMETA(DisplayName = "TikiGhost"),
	VolcanoGolem UMETA(DisplayName = "VolcanoGolem"),
	VolcanoLizard UMETA(DisplayName = "VolcanoLizard"),

	// Bosses
	DesertBoss UMETA(DisplayName = "DesertBoss"),
	ForestBoss UMETA(DisplayName = "ForestBoss"),
	HorrorBoss UMETA(DisplayName = "HorrorBoss"),
	IcelandBoss UMETA(DisplayName = "IcelandBoss"),
	ToylandBoss UMETA(DisplayName = "ToylandBoss"),
};

UENUM(BlueprintType)
enum class EEnemyTargetPriority : uint8
{
	None UMETA(DisplayName = "None"),
	CoreRune UMETA(DisplayName = "CoreRune"),
	Player UMETA(DisplayName = "Player"),
	Turret UMETA(DisplayName = "Turret"),
};