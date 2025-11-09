// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyAbilityData.generated.h"

class ASOWCharacterEnemyBase;

UENUM(BlueprintType)
enum class EEnemyAbility : uint8 {
	Cloaking,
	MagicShield,
	AntiMagic,
	DoubleTanker,
	None,
};

/**
 * 
 */
UCLASS()
class SOW_API UEnemyAbilityData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "EnemyType")
	TMap<TSubclassOf<ASOWCharacterEnemyBase>, EEnemyAbility> EnemyAbility;

public:
	UFUNCTION(BlueprintCallable)
	EEnemyAbility GetEnemyAbility(TSubclassOf<ASOWCharacterEnemyBase> EnemyType);
};
