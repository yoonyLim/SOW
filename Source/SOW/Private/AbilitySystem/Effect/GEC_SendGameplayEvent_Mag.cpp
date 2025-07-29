// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/GEC_SendGameplayEvent_Mag.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UGEC_SendGameplayEvent_Mag::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectApplied(ActiveGEContainer, GESpec, PredictionKey);
}

FGameplayEventData UGEC_SendGameplayEvent_Mag::MakeOugingGameplayEventData(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	const FGameplayEffectContextHandle& EffectContext = GESpec.GetEffectContext();
	AActor* SourceActor = EffectContext.GetOriginalInstigator(); // ¶Ç´Â GetInstigator()

	FGameplayEventData Data;
	Data.Instigator = SourceActor;
	Data.EventMagnitude = Duration.GetValue();
	return Data;
}
