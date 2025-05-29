// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyStructs.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct SOW_API FEnemyAttributeData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* HitAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* DeathAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AttackAnimation;
};
