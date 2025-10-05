// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_CalculateMeleeDamage.generated.h"

/**
 * 
 */
UCLASS()
class SOW_API UGEEC_CalculateMeleeDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
	float GetElementalResistanceCost(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FAggregatorEvaluateParameters EvalParams) const;

public:
	UGEEC_CalculateMeleeDamage();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
