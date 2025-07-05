// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_TurretHoppyBuff.generated.h"

/**
 * 
 */
UCLASS()
class SOW_API UGEEC_TurretHoppyBuff : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UGEEC_TurretHoppyBuff();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
