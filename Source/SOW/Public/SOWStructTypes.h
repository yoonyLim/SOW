// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "AbilitySystem/Ability/SOWPlayerGameplayAbility.h"
#include "SOWStructTypes.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPlayerAbilitySet {

	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<USOWPlayerGameplayAbility> AbilityToGrant;

	bool IsValid() const;
};
