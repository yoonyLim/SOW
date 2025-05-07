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

USTRUCT(BlueprintType)
struct FElementResistanceData : public FTableRowBase {
	GENERATED_BODY()

public:
	FElementResistanceData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Resistance Value"))
	float Nature;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Resistance Value"))
	float Flame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Resistance Value"))
	float Death;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Resistance Value"))
	float Ice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Resistance Value"))
	float Wave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Resistance Value"))
	float Divinity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Resistance Value"))
	float Madness;
};