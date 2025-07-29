// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Effect/GEC_SendGameplayEvent.h"
#include "GEC_SendGameplayEvent_Mag.generated.h"

/**
 * 
 */
UCLASS()
class SOW_API UGEC_SendGameplayEvent_Mag : public UGEC_SendGameplayEvent
{
	GENERATED_BODY()

public:

	virtual void OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;

	UPROPERTY(EditDefaultsOnly, Category = OnApplication)
	FScalableFloat Duration;
protected:
	virtual FGameplayEventData MakeOugingGameplayEventData(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;
};
