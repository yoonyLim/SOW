// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/SOWGameplayAbilityBase.h"
#include "SOWCoreRuneGameplayAbility.generated.h"

class ASOWCharacterCoreRune;
/**
 * 
 */
UCLASS()
class SOW_API USOWCoreRuneGameplayAbility : public USOWGameplayAbilityBase
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure)
	ASOWCharacterCoreRune* GetSOWCoreRuneCharacterFromActorInfo() const;
};
