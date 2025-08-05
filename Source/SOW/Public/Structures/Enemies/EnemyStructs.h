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

	/** 투사체 이동 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	bool bHasMovement = true;

	/** 투사체 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (EditCondition = "bHasMovement"))
	float Speed = 1200.f;

	/** 투사체 지속시간(초) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float Duration = 3.f;

	/** 투사체 스케일 배율 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ScaleRatio = 1.f;

	/** 데미지 이펙트 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/** 이펙트 레벨 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float EffectLevel = 1.f;
};