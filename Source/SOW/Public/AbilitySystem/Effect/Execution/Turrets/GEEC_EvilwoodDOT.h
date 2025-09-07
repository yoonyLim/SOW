// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_EvilwoodDOT.generated.h"

/**
 * 
 */
UCLASS()
class SOW_API UGEEC_EvilwoodDOT : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UGEEC_EvilwoodDOT();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
