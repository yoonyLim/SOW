// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/Components/ChanceToApplySineComponent.h"
#include "AbilitySystemComponent.h"
#include "SOWBlueprintFunctionLibrary.h"

bool UChanceToApplySineComponent::CanGameplayEffectApply(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const
{
	const FString ContextString = GESpec.Def->GetName();
	float AdditionalChance = 0.f;

	if (USOWBlueprintFunctionLibrary::DoesActorHasTag(GESpec.GetContext().GetInstigator(), ChangeConditionTag)) {
		AdditionalChance = 0.2f;
	}

	const float CalculatedChanceToApplyToTarget = GetChanceToApplyToTarget().GetValueAtLevel(GESpec.GetLevel(), &ContextString) + AdditionalChance;

	// check probability to apply
	if ((CalculatedChanceToApplyToTarget < 1.f - SMALL_NUMBER) && (FMath::FRand() > CalculatedChanceToApplyToTarget))
	{
		return false;
	}

	return true;
}

