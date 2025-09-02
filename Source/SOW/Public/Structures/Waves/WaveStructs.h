// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/SOWCharacterEnemyBase.h"
#include "WaveStructs.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct SOW_API FMonsterSpawnData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASOWCharacterEnemyBase> MonsterType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MonsterCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> SpawnPointIndices;
};

USTRUCT(BlueprintType)
struct SOW_API FWaveInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WaveNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMonsterSpawnData> MonsterSpawnData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InterludeDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaveDuration;
};