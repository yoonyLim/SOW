// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_ApplyTDOnSineCount.generated.h"

/**
 * 
 */



UCLASS()
class SOW_API UGEEC_ApplyTDOnSineCount : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UGEEC_ApplyTDOnSineCount();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};

void CheckDebuffStatus(AActor* TargetActor, FGameplayTag InTag, FGameplayTagContainer& TagCont, int32& SineCount);