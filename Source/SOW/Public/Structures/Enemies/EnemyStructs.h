// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"          // add
#include "GameplayEffect.h"           // add
#include "Enumerations/Enemies/EnemyEnums.h"
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
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackPowerBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefensePowerBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ShardDropAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ShardDropAmountVariation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEnemyTargetPriority TargetPriority;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* HitAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* DeathAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AttackAnimation;
};


USTRUCT(BlueprintType)
struct SOW_API FProjectileAttributeData : public FTableRowBase
{
	GENERATED_BODY()

	/** ����ü �̵� ���� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	bool bHasMovement = true;

	/** ����ü �ӵ� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (EditCondition = "bHasMovement"))
	float Speed = 1200.f;

	/** ����ü ���ӽð�(��) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float Duration = 3.f;

	/** ����ü ������ ���� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ScaleRatio = 1.f;

	/** ������ ����Ʈ Ŭ���� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/** ����Ʈ ���� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float EffectLevel = 1.f;
};