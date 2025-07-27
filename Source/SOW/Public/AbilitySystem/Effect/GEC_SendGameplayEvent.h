// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "SOWGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GEC_SendGameplayEvent.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ESendTarget : uint8 {
	Source,
	Target
};

UCLASS()
class SOW_API UGEC_SendGameplayEvent : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
	virtual void OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;

public:
	UPROPERTY(EditDefaultsOnly, Category = OnApplication)
	TArray<FGameplayTag> TagsToSend;

	UPROPERTY(EditDefaultsOnly, Category = OnApplication)
	ESendTarget TargetToSend = ESendTarget::Target;

protected:
	virtual FGameplayEventData MakeOugingGameplayEventData(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const;
};
