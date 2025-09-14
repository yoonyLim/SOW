// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_SyncSpeedWithAttributeEnemy.generated.h"

/**
 * 
 */
UCLASS()
class SOW_API UGEEC_SyncSpeedWithAttributeEnemy : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UGEEC_SyncSpeedWithAttributeEnemy();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
