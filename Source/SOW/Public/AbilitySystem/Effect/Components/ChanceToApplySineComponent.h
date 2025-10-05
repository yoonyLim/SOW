// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponents/ChanceToApplyGameplayEffectComponent.h"
#include "ChanceToApplySineComponent.generated.h"

struct FGameplayTag;
/**
 * 
 */
UCLASS(DisplayName = "Chance To Apply Debuff")
class SOW_API UChanceToApplySineComponent : public UChanceToApplyGameplayEffectComponent
{
	GENERATED_BODY()
	
public:
		virtual bool CanGameplayEffectApply(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const override;
private:
	/** Probability that this gameplay effect will be applied to the target actor (0.0 for never, 1.0 for always) */
	UPROPERTY(EditDefaultsOnly, Category = Application, meta = (GameplayAttribute = "True"))
	FGameplayTag ChangeConditionTag;
};
